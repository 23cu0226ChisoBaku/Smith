// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "SmithBattleSubsystem.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "SmithInventoryComponent.h"
#include "SmithAnimationComponent.h"
#include "SmithUpgradeInteractiveComponent.h"
#include "AttackHandle.h"
#include "SmithCommandFormat.h"
#include "Direction.h"
#include "FormatInfo_Import.h"
#include "MapObjType.h"
#include "SmithNextLevelEvent.h"
#include "SmithPickUpItemEvent.h"

#include "ICommandMediator.h"
#include "IEnhanceSystem.h"
#include "SmithHPItem.h"
#include "SmithUpgradeMaterial.h"

#include "HPUIComponent.h"
#include "SmithPlayerHP.h"
#include "SmithWeapon.h"

#include "ISmithBattleParameterizable.h"
#include "ISmithItemWidgetParameterizable.h"
#include "SmithUpgradeItemHandle.h"

#include "SmithBattleLogWorldSubsystem.h"

#include "BattleParamHandle.h"

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
	: CameraBoom(nullptr)
	, Camera(nullptr)
	, MoveComponent(nullptr)
	, AttackComponent(nullptr)
	, InventoryComponent(nullptr)
	, AnimationComponent(nullptr)
	, m_commandMediator(nullptr)
	, m_enhanceSystem(nullptr)
	, m_logSystem(nullptr)
	, m_curtHP(0)
	, m_maxHP(SmithPlayerActor::Private::PlayerHP_Temp)
	, m_rotateSpeed(720.0f)
	, m_rotatingDirection(0)
	, m_camDir(North)
	, m_actorFaceDir(North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
	, m_bRotatingCamera(false)
	, m_bIsInMenu(false)
{
	using namespace SmithPlayerActor::Private;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check((CameraBoom != nullptr))
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check((Camera != nullptr))
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetWorldRotation(FRotator::ZeroRotator);

	const FVector actorFwd = GetActorLocation().GetUnsafeNormal();
	if (actorFwd != FVector::ZeroVector)
	{
		m_camDir = VectorDirToEDir(actorFwd);
	}

	SetTurnPriority(ETurnPriority::PlayerSelf);

	MoveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("Smith MoveComponent"));
	check(::IsValid(MoveComponent));

	AttackComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("Smith AttackComponent"));
	check(::IsValid(AttackComponent));

	InventoryComponent = CreateDefaultSubobject<USmithInventoryComponent>(TEXT("Smith InventoryComponent"));
	check(::IsValid(InventoryComponent));

	AnimationComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("Smith AnimationComponent"));
	check(AnimationComponent != nullptr);

	HPComponent = CreateDefaultSubobject<UHPUIComponent>(TEXT("HP UI Component"));
	check(HPComponent != nullptr);

	UpgradeInteractiveComponent = CreateDefaultSubobject<USmithUpgradeInteractiveComponent>(TEXT("Smith UpgradeInteractiveComponent"));
	check(UpgradeInteractiveComponent != nullptr);

}

// Called when the game starts or when spawned
void ASmithPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
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

	// TODO
	if (Weapon == nullptr)
	{
		Weapon = NewObject<USmithWeapon>(this);
		check(Weapon != nullptr);
		if (Weapon != nullptr)
		{
			Weapon->SetParam(FParams{50, 10, 10, 10});
		}
	}
	
	Weapon->Rename(nullptr, this);

	{
		m_maxHP += Weapon->GetParam().HP;
		m_curtHP = m_maxHP;

		if (HPComponent != nullptr)
		{
			HPComponent->CreateHP(playerCtrl);
			HPComponent->SetHP(1.0f);
		}
	}

	{
		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			m_logSystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
		}
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
	if (m_curtHP <= 0)
	{

		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	if (m_bRotatingCamera)
	{
		using namespace SmithPlayerActor::Private;
		FRotator springArmNewRotator = CameraBoom->GetComponentRotation();
		const double targetAngle = StaticCast<double>(m_camDir) * Angle_Per_Direction;
		if (springArmNewRotator.Yaw < 0.0)
		{
			springArmNewRotator.Yaw += 360.0;
		}

		double leftAngle = targetAngle - springArmNewRotator.Yaw;
		if (FMath::Abs(leftAngle) > 90.0)
		{
			leftAngle += 360.0 * StaticCast<double>(m_rotatingDirection);
		}

		float rotateAngle = m_rotateSpeed * DeltaTime * StaticCast<float>(m_rotatingDirection);
		if (FMath::Abs(rotateAngle) > FMath::Abs(leftAngle))
		{
			rotateAngle = leftAngle;
			m_bRotatingCamera = false;
			m_rotatingDirection = 0;
			EnableInput(Cast<APlayerController>(Controller));
		}

		springArmNewRotator.Yaw += StaticCast<double>(rotateAngle);
		CameraBoom->SetWorldRotation(springArmNewRotator);
	}

}

// Called to bind functionality to input
void ASmithPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (::IsValid(inputComp))
	{
		inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASmithPlayerActor::Move_Input);
		inputComp->BindAction(CameraAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Look_Input);
		inputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Attack_Input);
		inputComp->BindAction(ChangeForwardAction, ETriggerEvent::Started, this, &ASmithPlayerActor::ChangeForward_Input);
		inputComp->BindAction(DebugAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Debug_SelfDamage_Input);
		inputComp->BindAction(MenuAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Menu_Input);
		inputComp->BindAction(SelectMenuAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Menu_Input_Select);
		inputComp->BindAction(InteractMenuAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Menu_Input_Interact);
	}
}

void ASmithPlayerActor::SetCommandMediator(ICommandMediator* mediator)
{
	m_commandMediator = mediator;
}

void ASmithPlayerActor::SetEnhanceSystem(IEnhanceSystem* enhanceSystem)
{
	m_enhanceSystem = enhanceSystem;
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

	float lookInput = value.Get<float>();

	uint8 newDir = StaticCast<uint8>(m_camDir);

	// TODO
	MDebug::Log(FString::SanitizeFloat(lookInput));
	if (lookInput > 0.0f)
	{
		newDir += East;
		m_rotatingDirection = 1;
	}
	else if (lookInput < 0.0f)
	{
		newDir += West;
		m_rotatingDirection = -1;
	}

	updateCamImpl(StaticCast<EDir_Test>(newDir % DirectionCnt));
}

void ASmithPlayerActor::ChangeForward_Input(const FInputActionValue& value)
{
	// TODO 同じ処理のため修正
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
}

void ASmithPlayerActor::Debug_SelfDamage_Input(const FInputActionValue& value)
{
	OnAttack(
						{ 
							this,		// Logger
							1000,		// Damage
						}
					);
}

void ASmithPlayerActor::Menu_Input(const FInputActionValue& value)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (UpgradeInteractiveComponent == nullptr)
	{
		return;
	}

	switchMenuStateImpl();
}

void ASmithPlayerActor::Menu_Input_Select(const FInputActionValue& value)
{
	if (UpgradeInteractiveComponent == nullptr)
	{
		return;
	}

	double inputValue = value.Get<float>();

	if (inputValue < 0)
	{
		UpgradeInteractiveComponent->SelectNextItem(ESelectDirection::Up);
	}
	else if (inputValue > 0)
	{
		UpgradeInteractiveComponent->SelectNextItem(ESelectDirection::Down);
	}
	else
	{
		MDebug::LogWarning("No input");
	}
}

void ASmithPlayerActor::Menu_Input_Interact(const FInputActionValue& value)
{

	if (!IsCommandSendable())
	{
		return;
	}

	if (UpgradeInteractiveComponent == nullptr || !m_bIsInMenu)
	{
		return;
	}

	int32 idx = UpgradeInteractiveComponent->GetSelectingItemIdx();

	enhanceImpl(idx);
}

void ASmithPlayerActor::moveImpl(EDirection direction)
{
	// 移動コマンドを出す
	if (::IsValid(MoveComponent) && m_commandMediator.IsValid())
	{
		m_commandMediator->SendMoveCommand(this, MoveComponent, direction, 1);
	}
}

void ASmithPlayerActor::attackImpl()
{
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
			FParams attackParam = Weapon->GetParam();
			const int32 atk = attackParam.ATK;

			FAttackHandle paramHandle;
			paramHandle.Attacker = this;
			paramHandle.AttackPower = attackParam.ATK;
			paramHandle.CriticalPower = attackParam.CRT;
			paramHandle.Level = Weapon->GetLevel();
			paramHandle.MotionValue = 1.0;
			m_commandMediator->SendAttackCommand(this, AttackComponent, StaticCast<EDirection>(m_actorFaceDir), *m_normalAttackFormatBuffer[attackKey], paramHandle);
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
	if (!::IsValid(CameraBoom))
	{
		return;
	}

	if (m_camDir == newDirection)
	{
		return;
	}

	m_bRotatingCamera = true;
	m_camDir = newDirection;
	DisableInput(Cast<APlayerController>(Controller));

}

void ASmithPlayerActor::switchMenuStateImpl()
{
	m_bIsInMenu = !m_bIsInMenu;

	APlayerController* playerCtrl = Cast<APlayerController>(Controller);
	check((playerCtrl != nullptr));

	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCtrl->GetLocalPlayer());
	check((enhancedInputSubsystem != nullptr));
	
	if (m_bIsInMenu)
	{
		enhancedInputSubsystem->RemoveMappingContext(MappingCtx);
		enhancedInputSubsystem->AddMappingContext(MappingCtx_Menu, 0);
		if (Weapon != nullptr)
		{
			UpgradeInteractiveComponent->SetWeaponInfo(Weapon->GetHandle());
		}

		if (InventoryComponent != nullptr)
		{
			TArray<UObject*> itemList;
			int32 cnt = InventoryComponent->GetAll(TEXT("UpgradeMaterial"), itemList);
			if (cnt > 0)
			{
				UpgradeInteractiveComponent->SetUpgradeItems(itemList);
			}
		} 
		UpgradeInteractiveComponent->ActivateUpgradeMenu();

	}
	else
	{
		enhancedInputSubsystem->RemoveMappingContext(MappingCtx_Menu);
		enhancedInputSubsystem->AddMappingContext(MappingCtx, 0);

		UpgradeInteractiveComponent->DeactivateUpgradeMenu();

	}

	if (HPComponent != nullptr)
	{
		HPComponent->SetWidgetVisibility(!m_bIsInMenu);
	}
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

void ASmithPlayerActor::enhanceImpl(int32 idx)
{
	if (m_enhanceSystem == nullptr || m_commandMediator == nullptr || InventoryComponent == nullptr)
	{
		return;
	}

	UObject* material = InventoryComponent->Get(TEXT("UpgradeMaterial"), idx);
	if (material == nullptr)
	{
		return;
	}

	{
		IParamAbsorbable* absorbItem = Cast<IParamAbsorbable>(material);
		if (absorbItem == nullptr)
		{
			return;
		}

		m_enhanceSystem->Enhance(Weapon, absorbItem);
		InventoryComponent->Remove(TEXT("UpgradeMaterial"), idx);
		m_commandMediator->SendIdleCommand(this);

		// TODO
		switchMenuStateImpl();
	}
}

void ASmithPlayerActor::OnAttack(AttackHandle&& attack)
{
	if (attack.AttackPower > 0)
	{
		m_curtHP -= attack.AttackPower;
		if (HPComponent != nullptr && m_maxHP > 0)
		{
			const float curtHPPercentage = StaticCast<float>(m_curtHP) / StaticCast<float>(m_maxHP);
			HPComponent->SetHP(curtHPPercentage);
		}

		if (m_logSystem != nullptr)
		{
			m_logSystem->SendAttackLog(attack.Attacker, this);
			m_logSystem->SendDamageLog(this, attack.AttackPower);
		}

	}
	else
	{
		return;
	}
	

	if (m_curtHP <= 0)
	{

		// TODO
		OnDead.ExecuteIfBound();
		if (AnimationComponent!= nullptr)
		{
			AnimationComponent->SwitchAnimState(TEXT("Dead"), 0.0f);
		}

		UWorld* world = GetWorld();
		if (::IsValid(world))
		{
			// ゲームを停めて、レベルをリロードする
			UGameplayStatics::SetGamePaused(world, true);
			UGameplayStatics::OpenLevel(world, FName(*(world->GetName())), false);
			DisableInput(Cast<APlayerController>(Controller));
		}

		// else
		// {
		// 	#if WITH_EDITOR
		// 		if (GEngine != nullptr)
		// 		{
		// 			UKismetSystemLibrary::QuitEditor();
		// 		}		
		// 	#else
		// 		//UKismetSystemLibrary::QuitGame();
		// 	#endif
		// }
	}
	else
	{
		if (AnimationComponent != nullptr)
		{
			AnimationComponent->SwitchAnimState(TEXT("Damaged"),0.0f);
			AnimationComponent->SwitchAnimStateDelay(TEXT("Idle"), 0.5f);
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

EMapObjType ASmithPlayerActor::GetType() const
{
	return EMapObjType::Player;
}

void ASmithPlayerActor::OnTriggerEvent(USmithNextLevelEvent* event)
{
	if (!::IsValid(event))
	{
		return;
	}

	// TODO?
	event->RaiseEvent();
}

void ASmithPlayerActor::OnTriggerEvent(USmithPickUpItemEvent* event)
{
	if (!::IsValid(event))
	{
		return;
	}

	if (InventoryComponent == nullptr)
	{
		return;
	}

	FString itemType = event->GetPickUpItemType();
	bool success = InventoryComponent->ContainsCategory(itemType) && !InventoryComponent->IsReachCapacity(itemType);

	if (m_logSystem != nullptr)
	{
		m_logSystem->SendInteractEventLog(this, event, success);
	}

	if (success)
	{
		IPickable* pickable = event->GetPickable();
		if (pickable != nullptr)
		{
			pickable->OnPick(this);
			event->RaiseEvent();
		}
	}
}

bool ASmithPlayerActor::PickUpConsume(USmithConsumeItem* consume)
{
	unimplemented();
	return false;
}

bool ASmithPlayerActor::PickUpMaterial(USmithUpgradeMaterial* upgrade)
{
	if (!::IsValid(upgrade))
	{
		MDebug::LogError("can not pick --- material invalid");
		return false;
	}

	return InventoryComponent->Insert(TEXT("UpgradeMaterial"), upgrade);
}

void ASmithPlayerActor::SwitchAnimation(uint8 animationState)
{
	if (AnimationComponent == nullptr)
	{
		return;
	}

	FName StateName;
	float durationTime;
	convertAnimState(animationState, StateName, durationTime);

	AnimationComponent->SwitchAnimState(StateName, durationTime);
}

void ASmithPlayerActor::SwitchAnimationDelay(uint8 animationState, float delay)
{
	if (AnimationComponent == nullptr)
	{
		return;
	}

	FName StateName;
	float dummy;
	convertAnimState(animationState, StateName, dummy);
	AnimationComponent->SwitchAnimStateDelay(StateName, delay);
}

void ASmithPlayerActor::UpdateAnimation(float deltaTime)
{
	if (AnimationComponent == nullptr)
	{
		return;
	}

	AnimationComponent->UpdateAnim(deltaTime);
}

bool ASmithPlayerActor::IsAnimationFinish() const
{
	return AnimationComponent == nullptr ? true : AnimationComponent->IsCurrentAnimationFinish();
}

void ASmithPlayerActor::convertAnimState(uint8 animationState, FName& outName, float& outDurationTime)
{
	using namespace UE::Smith;
	outName = TEXT("");
	outDurationTime = 0.0f;

	switch (animationState)
	{
		case SMITH_ANIM_IDLE:
			outName = TEXT("Idle");
			break;
		case	SMITH_ANIM_WALK:
			outName = TEXT("Walk");
			break;
		case SMITH_ANIM_ATTACK:
			outName = TEXT("Attack");
			outDurationTime = 1.0f;
			break;
		case SMITH_ANIM_DAMAGED:
			outName = TEXT("Damaged");
			break;
		case SMITH_ANIM_DEAD:
			outName = TEXT("Dead");
			break;
		default:
			break;
	}
}

FString ASmithPlayerActor::GetName_Log() const
{
	return TEXT("鍛冶師");
}

EBattleLogType ASmithPlayerActor::GetType_Log() const
{
	return EBattleLogType::Player;
}