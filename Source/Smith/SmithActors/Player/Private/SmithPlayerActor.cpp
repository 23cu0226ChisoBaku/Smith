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
#include "HerbWidget.h"

#include "SmithBattleGameInstanceSubsystem.h"

#include "MLibrary.h"

using namespace MLibrary::UE::Audio;

namespace SmithPlayerActor::Private
{
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
	, m_maxHP(0)
	, m_rotateSpeed(720.0f)
	, m_rotatingDirection(0)
	, m_turnCnt(0)
	, m_camDir(EDirection::North)
	, m_actorFaceDir(EDirection::North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
	, m_bRotatingCamera(false)
	, m_bIsInMenu(false)
	, m_bCanReceiveInput(true)
	, m_bIsDamaged(false)
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
	Weapon->OnUpgrade.AddUObject(this, &ASmithPlayerActor::updateParam);
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
			HPComponent->SetHPNumber(m_maxHP, m_maxHP);
		}
	}

	{
		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			m_logSystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
		}
	}

	if (InventoryComponent != nullptr)
	{
		for (int32 i = 0; i < 3; ++i)
		{
			USmithHPItem* item = NewObject<USmithHPItem>();
			// TODO Modelをいい感じに
			// 30%
			item->SetRecoveryPercentage(0.3);
			bool insertResult = InventoryComponent->Insert(TEXT("ConsumeItem"), item);
		}
	}

	if (HerbUISub != nullptr)
	{
		m_herbUI = CreateWidget<UHerbWidget>(GetWorld(), HerbUISub);
		if (m_herbUI != nullptr && InventoryComponent != nullptr) 
		{
			m_herbUI->AddToViewport();
			m_herbUI->SetNum(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
		}
	}

	m_actorFaceDir = EDirection::South;
	SetActorRotation(FRotator{0.0, 180.0, 0.0});

	OnTurnPass.AddUObject(this, &ASmithPlayerActor::turnPassRecover);
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Weapon != nullptr)
	{
		Weapon->OnUpgrade.Clear();
	}
}

// Called every frame
void ASmithPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// HPがなくなったらTickを停める
	if (m_curtHP <= 0)
	{
		return;
	}

	if (m_bIsDamaged)
	{
		if (AnimationComponent != nullptr)
		{
			AnimationComponent->UpdateAnim(DeltaTime);
			if (AnimationComponent->IsCurrentAnimationFinish())
			{
				m_bIsDamaged = false;
				m_bCanReceiveInput = true;
			}
		}
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
		AudioKit::PlaySE(TEXT("Player_Select_1"));
	}
	// 下のアイテムを選択
	else if (selectDirection > 0)
	{
		UpgradeInteractiveComponent->SelectNextItem(ESelectDirection::Down);
		AudioKit::PlaySE(TEXT("Player_Select_2"));
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
		bool success = m_commandMediator->SendMoveCommand(this, MoveComponent, newDirection, 1);
		if (!success)
		{
			if (AnimationComponent != nullptr)
			{
				FName outName;
				convertAnimState(UE::Smith::SMITH_ANIM_IDLE, outName);
				AnimationComponent->SwitchAnimState(outName);
			}
		}
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

		int32 ran = FMath::RandRange(1, 2);
		FString atkSE = TEXT("Player_Slash_") + FString::FromInt(ran);

		AudioKit::PlaySE(atkSE);
	}
}

void ASmithPlayerActor::ChangeForward(EDirection newDirection)
{
	if (!IsCommandSendable())
	{
		return;
	}

	changeForwardImpl(newDirection);
}

void ASmithPlayerActor::changeForwardImpl(EDirection newDirection)
{
	using namespace SmithPlayerActor::Private;

	m_actorFaceDir = newDirection;
	const double newYaw = StaticCast<double>(m_actorFaceDir) * ANGLE_PER_DIRECTION;
	SetActorRotation(FRotator{0.0, newYaw, 0.0});
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

		if (HPComponent != nullptr)
		{
			HPComponent->SetWidgetVisibility(!m_bIsInMenu);
		}

		if (m_herbUI != nullptr)
		{
			m_herbUI->SetVisibility(ESlateVisibility::Hidden);
		}
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
		if (HPComponent != nullptr)
		{
			HPComponent->SetWidgetVisibility(!m_bIsInMenu);
		}

		if (m_herbUI != nullptr)
		{
			m_herbUI->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void ASmithPlayerActor::RecoverHealth()
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (InventoryComponent == nullptr)
	{
		return;
	}

	UObject* consume = InventoryComponent->Get(TEXT("ConsumeItem"), 0);
	USmithConsumeItem* consumeItem = Cast<USmithConsumeItem>(consume);
	if (consumeItem == nullptr)
	{
		return;
	}

	consumeItem->Use(this);
	InventoryComponent->Remove(TEXT("ConsumeItem"), 0);
	
	if (m_herbUI != nullptr)
	{
		m_herbUI->SetNum(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendIdleCommand(this);
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

	AudioKit::PlaySE(TEXT("Hit_Iron_1"));

	// TODO
	CloseMenu();

	m_enhanceSystem->Enhance(Weapon, absorbItem);
	InventoryComponent->Remove(TEXT("UpgradeMaterial"), idx);
	m_commandMediator->SendIdleCommand(this);

	if (m_logSystem != nullptr)
	{
		m_logSystem->SendEnhanceLog(this);
	}
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
		if (attack.AttackFrom != EDirection::Invalid)
		{
			const EDirection newDir = StaticCast<EDirection>((StaticCast<uint8>(attack.AttackFrom) + 4u) % StaticCast<uint8>(EDirection::DirectionCount));
			changeForwardImpl(newDir);
		}

		m_bIsDamaged = true;
		m_bCanReceiveInput = false;
		m_curtHP -= attack.AttackPower;
		if (m_curtHP < 0)
		{
			m_curtHP = 0;
		}
		
		if (HPComponent != nullptr && m_maxHP > 0)
		{
			const float curtHPPercentage = StaticCast<float>(m_curtHP) / StaticCast<float>(m_maxHP);
			HPComponent->SetHP(curtHPPercentage);
			HPComponent->SetHPNumber(m_maxHP, m_curtHP);
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
		m_bCanReceiveInput = false;
		// TODO
		if (OnDead.IsBound())
		{
			OnDead.Broadcast();
		}

		if (AnimationComponent!= nullptr)
		{
			AnimationComponent->SwitchAnimState(TEXT("Dead"));
		}

		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			UGameInstance* gameInstance = world->GetGameInstance();
			USmithBattleGameInstanceSubsystem* battleInstanceSubsystem = gameInstance->GetSubsystem<USmithBattleGameInstanceSubsystem>();
			if (battleInstanceSubsystem != nullptr)
			{
				battleInstanceSubsystem->DisplayGameOverWidget(this);
			}
		}

	}
	else
	{
		if (AnimationComponent != nullptr)
		{
			AnimationComponent->SwitchAnimState(TEXT("Damaged"));
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

	event->RaiseEvent();
	m_actorFaceDir = EDirection::South;
	SetActorRotation(FRotator{0.0, 180.0, 0.0});
}

// TODO Refactoring
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
		if (IS_UINTERFACE_VALID(pickable))
		{
			InventoryComponent->Insert(pickable->GetPickType(), pickable->_getUObject());
			event->RaiseEvent();

			// TODO!!!!!!!
			// View専用クラスを作成し、コールバックを呼び出す
			// ActorはModel専用で！！！
			if (m_herbUI != nullptr)
			{
				m_herbUI->SetNum(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
			}
		}
	}
}

void ASmithPlayerActor::SwitchAnimation(uint8 animationState)
{
	if (AnimationComponent == nullptr)
	{
		return;
	}

	FName StateName;
	convertAnimState(animationState, StateName);

	AnimationComponent->SwitchAnimState(StateName);
}

void ASmithPlayerActor::UseItem(USmithHPItem* item)
{
	if (item == nullptr)
	{
		return;
	}

	const double recoveryPercentage = item->GetRecoveryPercentage();
	const int32 recovery = StaticCast<int32>(StaticCast<double>(m_maxHP) * recoveryPercentage);

	m_curtHP += recovery;
	if (m_curtHP > m_maxHP)
	{
		m_curtHP = m_maxHP;
	}

	const float curtHPPercentage = StaticCast<float>(m_curtHP) / StaticCast<float>(m_maxHP);
	if (HPComponent != nullptr)
	{
		HPComponent->SetHP(curtHPPercentage);
		HPComponent->SetHPNumber(m_maxHP, m_curtHP);
	}

	// TODO
	AudioKit::PlaySE(TEXT("Recover_1"));
}

void ASmithPlayerActor::SwitchAnimationDelay(uint8 animationState, float delay)
{
	if (AnimationComponent == nullptr)
	{
		return;
	}

	FName StateName;
	convertAnimState(animationState, StateName);
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

void ASmithPlayerActor::convertAnimState(uint8 animationState, FName& outName)
{
	using namespace UE::Smith;
	outName = TEXT("");

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
	OnAttack(AttackHandle{this, damage, EDirection::Invalid});
}

void ASmithPlayerActor::updateParam(FParams upgradeParam)
{
	if (upgradeParam.HP != 0)
	{
		const float curtPercentage = StaticCast<float>(m_curtHP) / StaticCast<float>(m_maxHP);

		m_maxHP += upgradeParam.HP;
		m_curtHP = FMath::CeilToInt32(StaticCast<float>(m_maxHP) * curtPercentage);

		if (HPComponent != nullptr)
		{
			HPComponent->SetHP(StaticCast<float>(m_curtHP) / StaticCast<float>(m_maxHP));
			HPComponent->SetHPNumber(m_maxHP, m_curtHP);
		}
	}
}

bool ASmithPlayerActor::CanReceiveInputEvent() const
{
	return m_bCanReceiveInput;
}

void ASmithPlayerActor::OnGameClear()
{
	m_bCanReceiveInput = false;
}

FBattleDefenseParamHandle ASmithPlayerActor::GetDefenseParam() const
{
	if (Weapon == nullptr)
	{
		return IAttackable::GetDefenseParam();		
	}

	FBattleDefenseParamHandle handle;
	FParams param = Weapon->GetParam();
	handle.DefensePoint = param.DEF;
	handle.Level = Weapon->GetLevel();

	return handle;
}

void ASmithPlayerActor::turnPassRecover()
{
	++m_turnCnt;
	if (m_turnCnt % 2 == 0)
	{
		if (m_curtHP < m_maxHP)
		{
			++m_curtHP;
			if (HPComponent != nullptr)
			{
				HPComponent->SetHP(StaticCast<float>(m_curtHP)/ StaticCast<float>(m_maxHP));
				HPComponent->SetHPNumber(m_maxHP, m_curtHP);
			}
		}
	}
}