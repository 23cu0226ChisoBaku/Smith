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
#include "TurnControlComponent.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "AttackCommand.h"
#include "MoveCommand.h"
#include "AttackHandle.h"

namespace SmithPlayerActor::Private
{
	constexpr int32 PlayerHP_Temp = 30;
	constexpr float TILE_SIZE = 250.0f;

	// Temp Capsule Size
	// TODO obsolete when map is done
	constexpr float CAPSULE_RADIUS = 90.0f;
	constexpr float CAPSULE_HALF_HEIGHT = 240.0f;
	// 方向の数につき隣の方向の間の角度
	constexpr double Angle_Per_Direction = 360.0 / (double)ASmithPlayerActor::EDir_Test::DirectionCnt;

	// ベクトルを方向列挙に変換する(X,Yだけ,Zは無視)
	ASmithPlayerActor::EDir_Test VectorDirToEDir(const FVector& direction)
	{
		const double dot = direction.Dot(FVector::ForwardVector);
		const FVector dir = direction.Cross(FVector::ForwardVector);

		// 外積のZによって180補正するかどうかを決める
		const double newAngle = FMath::RadiansToDegrees(acos(dot)) + (dir.Z > 0.0 ? 180.0 : 0.0); 

		return StaticCast<ASmithPlayerActor::EDir_Test>(round(newAngle / Angle_Per_Direction));
	}
}

ASmithPlayerActor::ASmithPlayerActor()
	: m_springArm(nullptr)
	, m_cam(nullptr)
	, m_turnComponent(nullptr)
	, m_moveComponent(nullptr)
	, m_atkComponent(nullptr)
	, m_event({})
	, m_hp(SmithPlayerActor::Private::PlayerHP_Temp)
	, m_camDir(North)
	, m_actorFaceDir(North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
{
	using namespace SmithPlayerActor::Private;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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

	m_turnComponent = CreateDefaultSubobject<UTurnControlComponent>(TEXT("TurnComponent"));
	check((m_turnComponent != nullptr));
	m_turnComponent->SetTurnPriority(ETurnPriority::PlayerSelf);

	m_moveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("Smith MoveComponent"));
	check((m_moveComponent != nullptr));
	m_moveComponent->SetMoveSpeed(TILE_SIZE);

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

	enhancedInputSubsystem->AddMappingContext(m_mappingCtx, 0);
	
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_event.Clear();
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
		inputComp->BindAction(m_moveAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Move_Input);
		inputComp->BindAction(m_attackAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Attack_Input);
		inputComp->BindAction(m_cameraAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Look_Input);
		inputComp->BindAction(m_debugAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Debug_SelfDamage_Input);
	}
}

UTurnControlComponent* ASmithPlayerActor::GetTurnControl() const
{
  return m_turnComponent;
}

FDelegateHandle ASmithPlayerActor::Subscribe(FRequestCommandEvent::FDelegate& delegate)
{
	if (delegate.IsBound())
	{
		return m_event.Add(delegate);
	}

	return delegate.GetHandle();
}

bool ASmithPlayerActor::Unsubscribe(UObject* obj, FDelegateHandle delegateHandle)
{
	if (obj != nullptr && m_event.IsBoundToObject(obj))
	{
		m_event.Remove(delegateHandle);
		return true;
	}
	else
	{
		return false;
	}
}

void ASmithPlayerActor::Move_Input(const FInputActionValue& value)
{
	using namespace SmithPlayerActor::Private;

	if (!m_bCanMove)
	{
		return;
	}

	if (!::IsValid(m_turnComponent) || !m_turnComponent->IsCommandSendable())
	{
		return;
	}

	const FVector2D movementInput = value.Get<FVector2D>();
	if (movementInput == FVector2D::ZeroVector)
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
	moveImpl(FVector{directionX, directionY, 0.0});
}

void ASmithPlayerActor::Attack_Input(const FInputActionValue& value)
{
	if (!m_bCanAttack)
	{
		return;
	}

	if (!::IsValid(m_turnComponent) || !m_turnComponent->IsCommandSendable())
	{
		return;
	}

	attackImpl();
}

// TODO Test
void ASmithPlayerActor::Look_Input(const FInputActionValue& value)
{
	const FVector2D lookInput = value.Get<FVector2D>();

	if (lookInput == FVector2D::ZeroVector)
	{
		return;
	}

	uint8 newDir = StaticCast<uint8>(m_camDir);

	if (lookInput.X > 0.0f)
	{
		newDir += East;
	}
	else if (lookInput.X < 0.0f)
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

void ASmithPlayerActor::sendCommand(TSharedPtr<IBattleCommand> command)
{
	if (command == nullptr)
	{
		return;
	}

	auto turnComp = GetTurnControl();
	if (turnComp == nullptr || !turnComp->IsCommandSendable())
	{
		return;
	}

	if (!m_event.IsBound())
	{
		return;
	}

	m_event.Broadcast(this, command);

}

void ASmithPlayerActor::moveImpl(FVector moveDir)
{
	using namespace SmithPlayerActor::Private;

	FHitResult hit{};

	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + moveDir * TILE_SIZE;

	FCollisionQueryParams hitParam;
	hitParam.AddIgnoredActor(this);
	// TODO temp size
	const float sphereRadius = CAPSULE_HALF_HEIGHT * 0.5f;

	const bool isHit = GetWorld()->SweepSingleByChannel(
																											hit, 
																											GetActorLocation(),
																											endPos,
																											FQuat::Identity,
																											ECollisionChannel::ECC_MAX,
																											FCollisionShape::MakeSphere(sphereRadius),
																											hitParam
																										 );

	// TODO 何もヒットしない場合移動コマンドを出す
	if (!isHit && ::IsValid(m_moveComponent))
	{
		m_moveComponent->SetTerminusPos(endPos);
		sendCommand(MakeShared<UE::Smith::Command::MoveCommand>(m_moveComponent));
	}
}

void ASmithPlayerActor::attackImpl()
{
	TArray<AActor*> hitActors {};

	const FVector atkDir = GetActorForwardVector();

	// TODO
	// 前一マスにActorを探す
	if (searchActorsInDirection(atkDir, hitActors))
	{
		// 攻撃できる相手を攻撃コマンドに登録
		for(auto actorPtr : hitActors)
		{
			IAttackable* attackable = Cast<IAttackable>(actorPtr);
			sendCommand(MakeShared<UE::Smith::Command::AttackCommand>(m_atkComponent, attackable, AttackHandle{GetName(), 3}));
		}
	}
	// なかったら空振りする
	else
	{
		sendCommand(MakeShared<UE::Smith::Command::AttackCommand>(nullptr, nullptr, AttackHandle{}));
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

bool ASmithPlayerActor::searchActorsInDirection(FVector direction, TArray<AActor*>& OutResult)
{
	using namespace SmithPlayerActor::Private;

	OutResult.Empty();

	TArray<FHitResult> hits{};

	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + direction * TILE_SIZE;

	FCollisionQueryParams hitParam;
	hitParam.AddIgnoredActor(this);

	// TODO temp code
	const float sphereRadius = CAPSULE_HALF_HEIGHT * 0.5f;
	
	const bool isHit = GetWorld()->SweepMultiByChannel(
																											hits, 
																											GetActorLocation(),
																											endPos,
																											FQuat::Identity,
																											ECollisionChannel::ECC_MAX,
																											FCollisionShape::MakeSphere(sphereRadius),
																											hitParam
																										);

	if (isHit)
	{
		for(const auto& hitResult: hits)
		{
			OutResult.Emplace(hitResult.GetActor());
		}
	}

	return isHit;
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

		// ゲームを停めて、レベルをリロードする
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*(GetWorld()->GetName())), false);
		DisableInput(Cast<APlayerController>(Controller));

	}
}