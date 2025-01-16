// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SmithBattleSubsystem.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "AttackCommand.h"
#include "MoveCommand.h"
#include "AttackHandle.h"
#include "SmithCommandFormat.h"
#include "FormatType.h"
#include "Direction.h"
#include "FormatInfo_Import.h"

#include "ICommandMediator.h"

#include "MLibrary.h"

namespace SmithPlayerActor::Private
{
	#define DIRECTION_ENUM ASmithPlayerActor::EDir_Test
	constexpr int32 PlayerHP_Temp = 30;

	// 方向の数につき隣の方向の間の角度
	constexpr double Angle_Per_Direction = 360.0 / (double)DIRECTION_ENUM::DirectionCnt;

	// ベクトルを方向列挙に変換する(X,Yだけ,Zは無視)
	DIRECTION_ENUM VectorDirToEDir(const FVector& direction)
	{
		const double dot = direction.Dot(FVector::ForwardVector);
		const FVector cross = direction.Cross(FVector::ForwardVector);

		const double angle = FMath::RadiansToDegrees(acos(dot));
		// (1,0,0)(※EDir_Test::Northを表すベクトル)から時計回りに回転しdirectionまで回転した角度を計算
		const double angleClockwise =  cross.Z > 0.0 ? (360.0 - angle) : angle; 
		const DIRECTION_ENUM dir = StaticCast<DIRECTION_ENUM>(StaticCast<uint8>(round(angleClockwise / Angle_Per_Direction)) % DIRECTION_ENUM::DirectionCnt);
		return dir;
	}
	
	#undef DIRECTION_ENUM
}

ASmithPlayerActor::ASmithPlayerActor()
	: m_springArm(nullptr)
	, m_cam(nullptr)
	, m_moveComponent(nullptr)
	, m_atkComponent(nullptr)
	, m_commandMediator(nullptr)
	, m_hp(SmithPlayerActor::Private::PlayerHP_Temp)
	, m_camDir(North)
	, m_actorFaceDir(North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
{
	using namespace SmithPlayerActor::Private;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check((m_springArm != nullptr))
	m_springArm->SetupAttachment(RootComponent);

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check((m_cam != nullptr))
	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);

	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->SetUsingAbsoluteRotation(true);
	m_springArm->SetWorldRotation(FRotator::ZeroRotator);
	m_springArm->TargetArmLength = 1500.0f;

	const FVector actorFwd = GetActorLocation().GetUnsafeNormal();
	if (actorFwd != FVector::ZeroVector)
	{
		m_camDir = VectorDirToEDir(actorFwd);
	}

	SetTurnPriority(ETurnPriority::PlayerSelf);

	m_moveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("Smith MoveComponent"));
	check((m_moveComponent != nullptr));

	m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("Smith AttackComponent"));
	check((m_atkComponent != nullptr));

}

// Called when the game starts or when spawned
void ASmithPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO BPで設定できるようにする
	if (::IsValid(m_springArm))
	{
		m_springArm->SetWorldRotation(FRotator{300.0, 0.0, 0.0});
	}

	// Mapping Contextを設定
	APlayerController* playerCtrl = Cast<APlayerController>(Controller);
	check((playerCtrl != nullptr));

	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCtrl->GetLocalPlayer());
	check((enhancedInputSubsystem != nullptr));

	enhancedInputSubsystem->AddMappingContext(MappingCtx, 0);
	// TODO
	for (auto& pair : AttackFormatTables)
	{
		if (!pair.Value.IsValid())
		{
			pair.Value.LoadSynchronous();
		}

		registerAttackFormat(pair.Key, pair.Value.Get());
	}
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASmithPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HPがなくなったらTickを停める
	if (m_hp <= 0)
	{
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

}

// Called to bind functionality to input
void ASmithPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (::IsValid(inputComp))
	{
		inputComp->BindAction(MoveAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Move_Input);
		inputComp->BindAction(CameraAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Look_Input);
		inputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Attack_Input);
		inputComp->BindAction(DebugAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Debug_SelfDamage_Input);
	}
}

void ASmithPlayerActor::SetCommandMediator(ICommandMediator* mediator)
{
	m_commandMediator = mediator;
}

void ASmithPlayerActor::Move_Input(const FInputActionValue& value)
{
	if (!IsCommandSendable())
	{
		return;
	}

	using namespace SmithPlayerActor::Private;

	FVector2D movementInput = value.Get<FVector2D>();
	movementInput.Normalize();
	if (movementInput.IsNearlyZero())
	{
		return;
	}

	// カメラの角度で回転ベクトルを計算する
	const double cameraAngle = FMath::DegreesToRadians(StaticCast<double>(m_camDir) * Angle_Per_Direction);
	double directionX = movementInput.Y * cos(cameraAngle) - movementInput.X * sin(cameraAngle);
	double directionY = movementInput.Y * sin(cameraAngle) + movementInput.X * cos(cameraAngle);

	// TODO ゼロ補正
	if (FMath::IsNearlyZero(directionX))
	{
		directionX = 0.0;
	}

	if (FMath::IsNearlyZero(directionY))
	{
		directionY = 0.0;
	}

	const EDir_Test newDir = VectorDirToEDir(FVector{directionX, directionY, 0.0});
	changeFwdImpl(newDir);
	moveImpl(StaticCast<EDirection>(newDir));
}

void ASmithPlayerActor::Attack_Input(const FInputActionValue& value)
{
	if (!IsCommandSendable())
	{
		return;
	}

	attackImpl();
}

void ASmithPlayerActor::Look_Input(const FInputActionValue& value)
{
	if (!IsCommandSendable())
	{
		return;
	}

	FVector2D lookInput = value.Get<FVector2D>();
	lookInput.Normalize();
	if (lookInput.IsNearlyZero())
	{
		return;
	}

	uint8 newDir = StaticCast<uint8>(m_camDir);

	// TODO
	if (lookInput.X > 0.9f)
	{
		newDir += East;
	}
	else if (lookInput.X < -0.9f)
	{
		newDir += West;
	}

	updateCamImpl(StaticCast<EDir_Test>(newDir % DirectionCnt));
}

void ASmithPlayerActor::Debug_SelfDamage_Input(const FInputActionValue& value)
{
	OnAttack(
						{ 
							TEXT("God"),		// Attack
						  10,							// Damage
						}
					);
}

void ASmithPlayerActor::moveImpl(EDirection direction)
{
	// 移動コマンドを出す
	if (::IsValid(m_moveComponent) && m_commandMediator.IsValid())
	{
		m_commandMediator->SendMoveCommand(this, m_moveComponent, direction, 1);
	}
}

void ASmithPlayerActor::attackImpl()
{
	// TODO deep nest is bad
	if (m_commandMediator.IsValid())
	{
		FString attackKey = TEXT("");
		// 斜めだったら
		if (m_actorFaceDir % 2 == 1)
		{
			attackKey = TEXT("AttackDiagonal");
		}
		else
		{
			attackKey = TEXT("Attack");
		}

		if (m_normalAttackFormatBuffer.Contains(attackKey) && m_normalAttackFormatBuffer[attackKey].IsValid())
		{
			m_commandMediator->SendAttackCommand(this, m_atkComponent, StaticCast<EDirection>(m_actorFaceDir), *m_normalAttackFormatBuffer[attackKey], AttackHandle{GetName(), 3});
		}
	}
}

void ASmithPlayerActor::changeFwdImpl(EDir_Test newDirection)
{
	using namespace SmithPlayerActor::Private;

	// 今の向きと変えようとする向きが同じだったら処理しない
	if (m_actorFaceDir == newDirection)
	{
		return;
	}

	m_actorFaceDir = newDirection;
	const double newYaw = StaticCast<double>(m_actorFaceDir) * Angle_Per_Direction;
	SetActorRelativeRotation(FRotator(0.0, newYaw, 0.0));
}

void ASmithPlayerActor::updateCamImpl(EDir_Test newDirection)
{
	using namespace SmithPlayerActor::Private;
	if (!::IsValid(m_springArm))
	{
		return;
	}

	if (m_camDir == newDirection)
	{
		return;
	}

	m_camDir = newDirection;
	FRotator springArmNewRotator = m_springArm->GetRelativeRotation();
	const double springArmRotateY = StaticCast<double>(m_camDir) * Angle_Per_Direction;
	springArmNewRotator.Yaw = springArmRotateY;
	m_springArm->SetWorldRotation(springArmNewRotator);
}

bool ASmithPlayerActor::registerAttackFormat(const FString& name, const UDataTable* formatTable)
{
	if (m_normalAttackFormatBuffer.Contains(name))
  {
    return false;
  }

  if (formatTable == nullptr)
  {
    return false;
  }

	TArray<FFormatInfo_Import*> arr;
	formatTable->GetAllRows<FFormatInfo_Import>("", arr);

	if (arr.Num() <= 1)
	{
		return false;
	}

	TArray<FName> names = formatTable->GetRowNames();
	if (!names[0].IsEqual(TEXT("FormatInfo")))
	{
		return false;
	}
	
	const uint8 formatRow = arr[0]->Row;
	const uint8 formatColumn = arr[0]->Column;
	const size_t dataCnt = formatRow * formatColumn;

	check((arr.Num() - 1) == StaticCast<int32>(dataCnt));
	if ((arr.Num() - 1) != StaticCast<int32>(dataCnt))
	{
		return false;
	}

	TArray<ESmithFormatType> typeSrcData;

	for (int i = 1; i < arr.Num(); ++i)
	{
		typeSrcData.Emplace(arr[i]->Type);
	}

	TSharedPtr<UE::Smith::Battle::FSmithCommandFormat> formatPtr = ::MakeShared<UE::Smith::Battle::FSmithCommandFormat>(typeSrcData.GetData(), dataCnt, formatRow, formatColumn);
	m_normalAttackFormatBuffer.Emplace(name, formatPtr);

	return true;
}

void ASmithPlayerActor::OnAttack(AttackHandle&& attack)
{
	m_hp -= attack.AttackPower;

	FString msg{};
	msg.Append(GetName());
	msg.Append(TEXT(" left HP:"));
	msg.Append(FString::FromInt(m_hp));
	MDebug::LogError(msg);

	if (m_hp <= 0)
	{
		MDebug::LogError(TEXT("Player is dead"));

		UWorld* world = GetWorld();
		if (::IsValid(world))
		{
			// ゲームを停めて、レベルをリロードする
			UGameplayStatics::SetGamePaused(world, true);
			UGameplayStatics::OpenLevel(world, FName(*(world->GetName())), false);
			DisableInput(Cast<APlayerController>(Controller));
		}
		else
		{
			#if WITH_EDITOR
				if (GEngine != nullptr)
				{
					UKismetSystemLibrary::QuitEditor();
				}		
			#else
				//UKismetSystemLibrary::QuitGame();
			#endif
		}

	}
}

uint8 ASmithPlayerActor::GetOnMapSizeX() const
{
	return 1;
}

uint8 ASmithPlayerActor::GetOnMapSizeY() const
{
	return 1;
}