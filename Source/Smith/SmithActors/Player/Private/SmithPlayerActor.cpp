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
	constexpr int32 PlayerHP_Temp = 30;
	constexpr double ANGLE_PER_DIRECTION = 360.0 / (double)EDirection::DirectionCount;
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
	, m_camDir(EDirection::North)
	, m_actorFaceDir(EDirection::North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
	, m_bRotatingCamera(false)
	, m_bIsInMenu(false)
{
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
			// TODO
			APlayerController* playerCtrl = Cast<APlayerController>(Controller);
			check((playerCtrl != nullptr));
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
		updateCamera(DeltaTime);
	}

}

// Called to bind functionality to input
void ASmithPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASmithPlayerActor::SetCommandMediator(ICommandMediator* mediator)
{
	m_commandMediator = mediator;
}

void ASmithPlayerActor::SetEnhanceSystem(IEnhanceSystem* enhanceSystem)
{
	m_enhanceSystem = enhanceSystem;
}

void ASmithPlayerActor::SelectNextMenuItem(float selectDirection)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (!m_bIsInMenu)
	{
		return;
	}

	if (UpgradeInteractiveComponent == nullptr)
	{
		return;
	}

	// 上のアイテムを選択
	if (selectDirection < 0)
	{
		UpgradeInteractiveComponent->SelectNextItem(ESelectDirection::Up);
	}
	// 下のアイテムを選択
	else if (selectDirection > 0)
	{
		UpgradeInteractiveComponent->SelectNextItem(ESelectDirection::Down);
	}
}
bool ASmithPlayerActor::InteractMenu()
{
	if (!IsCommandSendable())
	{
		return false;
	}

	if (UpgradeInteractiveComponent == nullptr || !m_bIsInMenu)
	{
		return false;
	}

	int32 idx = UpgradeInteractiveComponent->GetSelectingItemIdx();

	return enhanceImpl(idx);
}

void ASmithPlayerActor::Move(EDirection newDirection)
{
	if (!IsCommandSendable())
	{
		return;
	}

	// 移動する前に向きを変える
	ChangeForward(newDirection);
	// 移動コマンドを出す
	if (::IsValid(MoveComponent) && m_commandMediator.IsValid())
	{
		m_commandMediator->SendMoveCommand(this, MoveComponent, newDirection, 1);
	}
}

void ASmithPlayerActor::Attack()
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_commandMediator.IsValid())
	{
		FString attackKey = TEXT("");
		// 斜めだったら
		if (StaticCast<uint8>(m_actorFaceDir) % 2 == 1)
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

void ASmithPlayerActor::ChangeForward(EDirection newDirection)
{
	if (!IsCommandSendable())
	{
		return;
	}

	using namespace SmithPlayerActor::Private;
	// 今の向きと変えようとする向きが同じだったら処理しない
	if (m_actorFaceDir == newDirection)
	{
		return;
	}

	m_actorFaceDir = newDirection;
	const double newYaw = StaticCast<double>(m_actorFaceDir) * ANGLE_PER_DIRECTION;
	SetActorRelativeRotation(FRotator(0.0, newYaw, 0.0));
}

void ASmithPlayerActor::ChangeCameraDirection(EDirection newDirection, bool bIsClockwise)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (!::IsValid(CameraBoom))
	{
		return;
	}

	m_rotatingDirection = bIsClockwise ? 1 : -1;
	m_bRotatingCamera = true;
	m_camDir = newDirection;
	
	if (OnStartCameraRotation.IsBound())
	{
		OnStartCameraRotation.Broadcast();
	}

}

void ASmithPlayerActor::OpenMenu()
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (UpgradeInteractiveComponent == nullptr)
	{
		return;
	}

	if (!m_bIsInMenu)
	{
		m_bIsInMenu = true;
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

	if (HPComponent != nullptr)
	{
		HPComponent->SetWidgetVisibility(!m_bIsInMenu);
	}
}

void ASmithPlayerActor::CloseMenu()
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (UpgradeInteractiveComponent == nullptr)
	{
		return;
	}

	if (m_bIsInMenu)
	{
		m_bIsInMenu = false;
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

bool ASmithPlayerActor::enhanceImpl(int32 idx)
{
	if (m_enhanceSystem == nullptr || m_commandMediator == nullptr || InventoryComponent == nullptr)
	{
		return false;
	}

	UObject* material = InventoryComponent->Get(TEXT("UpgradeMaterial"), idx);
	if (material == nullptr)
	{
		return false;
	}

	IParamAbsorbable* absorbItem = Cast<IParamAbsorbable>(material);
	if (absorbItem == nullptr)
	{
		return false;
	}

	// TODO
	CloseMenu();

	m_enhanceSystem->Enhance(Weapon, absorbItem);
	InventoryComponent->Remove(TEXT("UpgradeMaterial"), idx);
	m_commandMediator->SendIdleCommand(this);

	return true;
	
}

void ASmithPlayerActor::updateCamera(float deltaTime)
{
	using namespace SmithPlayerActor::Private;
	FRotator springArmNewRotator = CameraBoom->GetComponentRotation();
	const double targetAngle = StaticCast<double>(m_camDir) * ANGLE_PER_DIRECTION;
	if (springArmNewRotator.Yaw < 0.0)
	{
		springArmNewRotator.Yaw += 360.0;
	}

	double leftAngle = targetAngle - springArmNewRotator.Yaw;
	if (FMath::Abs(leftAngle) > 90.0)
	{
		leftAngle += 360.0 * StaticCast<double>(m_rotatingDirection);
	}

	float rotateAngle = m_rotateSpeed * deltaTime * StaticCast<float>(m_rotatingDirection);
	if (FMath::Abs(rotateAngle) > FMath::Abs(leftAngle))
	{
		rotateAngle = leftAngle;
		m_bRotatingCamera = false;
		m_rotatingDirection = 0;
		if (OnFinishCameraRotation.IsBound())
		{
			OnFinishCameraRotation.Broadcast();
		}
	}

	springArmNewRotator.Yaw += StaticCast<double>(rotateAngle);
	CameraBoom->SetWorldRotation(springArmNewRotator);		
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

EDirection ASmithPlayerActor::GetCameraDirection() const
{
	return m_camDir;
}

void ASmithPlayerActor::SelfDamage_Debug(int32 damage)
{
	m_curtHP -= damage;
}