// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerActor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Smithゲーム用コンポネント
#include "SmithInventoryComponent.h"
#include "SmithAnimationComponent.h"

#include "AttackHandle.h"
#include "BattleParamHandle.h"
#include "SmithCommandFormat.h"
#include "FormatInfo_Import.h"
#include "MapObjType.h"
#include "MinimapDisplayType.h"

// イベント
#include "SmithNextLevelEvent.h"
#include "SmithPickUpItemEvent.h"

// アイテム
#include "SmithHPItem.h"

#include "SmithWeapon.h"
#include "SmithBattleLogWorldSubsystem.h"

// インターフェース
#include "ICommandMediator.h"
#include "IEnhanceSystem.h"
#include "ParamAbsorbable.h"
#include "ISmithBattleParameterizable.h"
#include "ISmithItemWidgetParameterizable.h"

// 自作ライブラリ
#include "MLibrary.h"

using namespace MLibrary::UE::Audio;

namespace SmithPlayerActor::Private
{
  constexpr double ANGLE_PER_DIRECTION = 360.0 / (double)EDirection::DirectionCount;
}

ASmithPlayerActor::ASmithPlayerActor()
  : CameraBoom(nullptr)
  , Camera(nullptr)
  , InventoryComponent(nullptr)
  , AnimationComponent(nullptr)
  , m_commandMediator(nullptr)
  , m_enhanceSystem(nullptr)
  , m_logSender(nullptr)
  , m_currentHealth(0)
  , m_maxHealth(0)
  , m_turnCnt(0)
  , m_camDir(EDirection::North)
  , m_actorFaceDir(EDirection::South)
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

  InventoryComponent = CreateDefaultSubobject<USmithInventoryComponent>(TEXT("Smith InventoryComponent"));
  check(::IsValid(InventoryComponent));

  AnimationComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("Smith AnimationComponent"));
  check(AnimationComponent != nullptr);

}

// Called when the game starts or when spawned
void ASmithPlayerActor::BeginPlay()
{
  Super::BeginPlay();
  
  // 攻撃フォーマットを登録
  for (const auto& [formatName, formatDataTable] : AttackFormatTables)
  {
    if (!formatDataTable.IsValid())
    {
      formatDataTable.LoadSynchronous();
    }
    registerAttackFormat(formatName, formatDataTable.Get());
  }

  // 装備初期化
  {
    if (Weapon == nullptr)
    {
      Weapon = NewObject<USmithWeapon>(this);
      Weapon->SetParam(FParams{50, 10, 10, 10});
    }
  
    Weapon->OnParamUpdated.AddUObject(this, &ASmithPlayerActor::updateParam);
    Weapon->Rename(nullptr, this);
  }

  // HP初期化
  {
    m_maxHealth += Weapon->GetParam().HP;
    m_currentHealth = m_maxHealth;

    notifyHealthChanged();
  }

  // ログシステム初期化
  {
    UWorld* world = GetWorld();
    if (world != nullptr)
    {
      m_logSender = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
    }
  }

  // TODO 初期に持たせる薬草
  if (InventoryComponent != nullptr)
  {
    for (int32 i = 0; i < 3; ++i)
    {
      USmithHPItem* item = NewObject<USmithHPItem>();
      // TODO 初期化する方法を変える
      // 40%
      item->SetRecoveryPercentage(0.4);
      bool insertResult = InventoryComponent->Insert(TEXT("ConsumeItem"), item);
    }

    if (OnHerbValueChanged.IsBound())
    {
      OnHerbValueChanged.Broadcast(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
    }
  }

  m_actorFaceDir = EDirection::South;
  SetActorRotation(FRotator{0.0, 180.0, 0.0});

  // ターン優先順位を設定
  SetTurnPriority(ETurnPriority::PlayerSelf);
  OnTurnPass.AddUObject(this, &ASmithPlayerActor::turnPassRecover);
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  if (Weapon != nullptr)
  {
    Weapon->OnParamUpdated.Clear();
  }
}

// Called every frame
void ASmithPlayerActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  // HPがなくなったらTickを停める
  if (m_currentHealth <= 0)
  {
    return;
  }

  // ダメージアニメーション再生中操作停止
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
}

void ASmithPlayerActor::SetCommandMediator(ICommandMediator* mediator)
{
  m_commandMediator = mediator;
}

void ASmithPlayerActor::SetEnhanceSystem(IEnhanceSystem* enhanceSystem)
{
  m_enhanceSystem = enhanceSystem;
}

void ASmithPlayerActor::SelectNextMenuItem(float SelectDirection)
{
  if (!IsCommandSendable() || !m_bIsInMenu)
  {
    return;
  }

  if (OnMenuItemChangeFocus.IsBound())
  {
    OnMenuItemChangeFocus.Broadcast(SelectDirection);
  }
}
void ASmithPlayerActor::InteractMenu()
{
  if (!IsCommandSendable() || !m_bIsInMenu)
  {
    return;
  }

  if (OnItemSelected.IsBound())
  {
    const int32 selectedItemIdx = OnItemSelected.Execute();
    if (selectedItemIdx != -1)
    {
      if (enhanceImpl(selectedItemIdx))
      {
        CloseMenu();
      }
    }
  }
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
  if (m_commandMediator.IsValid())
  {
    bool success = m_commandMediator->SendMoveCommand(this, newDirection, 1);
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
  if (!IsCommandSendable() || !m_commandMediator.IsValid())
  {
    return;
  }

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
    paramHandle.AttackPower = attackParam.ATK;
    paramHandle.Level = Weapon->GetLevel();
    paramHandle.MotionValue = 1.0;
    m_commandMediator->SendAttackCommand(this, StaticCast<EDirection>(m_actorFaceDir), *m_normalAttackFormatBuffer[attackKey], paramHandle);
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

void ASmithPlayerActor::OpenMenu()
{
  if (!IsCommandSendable() || m_bIsInMenu)
  {
    return;
  }

  m_bIsInMenu = true;

  if ((Weapon != nullptr) && (InventoryComponent != nullptr))
  {
    // TODO Bad Hard Coding
    TArray<UObject*> itemList;
    int32 cnt = InventoryComponent->GetAll(TEXT("UpgradeMaterial"), itemList);

    if (OnEnhanceMenuInitialized.IsBound())
    {
      OnEnhanceMenuInitialized.Execute(Weapon->GetHandle(), itemList);
    }
  }

  if (OnEnhanceMenuOpened.IsBound())
  {
    OnEnhanceMenuOpened.Broadcast();
  }

}

void ASmithPlayerActor::CloseMenu()
{
  if (!IsCommandSendable() || !m_bIsInMenu)
  {
    return;
  }

  m_bIsInMenu = false;

  if (OnEnhanceMenuClosed.IsBound())
  {
    OnEnhanceMenuClosed.Broadcast();
  }
}

void ASmithPlayerActor::RecoverHealth()
{
  if (!IsCommandSendable())
  {
    return;
  }

  if (!m_commandMediator.IsValid() || (InventoryComponent == nullptr))
  {
    return;
  }

  const FString consumeInventoryName = TEXT("ConsumeItem");
  UObject* consume = InventoryComponent->Get(consumeInventoryName, 0);
  USmithConsumeItem* consumeItem = Cast<USmithConsumeItem>(consume);
  if (consumeItem == nullptr)
  {
    return;
  }

  consumeItem->Use(this);
  InventoryComponent->Remove(consumeInventoryName, 0);
  
  if (OnHerbValueChanged.IsBound())
  {
    OnHerbValueChanged.Broadcast(InventoryComponent->GetQuantity(consumeInventoryName));
  }

  m_commandMediator->SendIdleCommand(this);
  
}

bool ASmithPlayerActor::registerAttackFormat(const FString& name, const UDataTable* formatTable)
{
  check(formatTable != nullptr)
  if (m_normalAttackFormatBuffer.Contains(name))
  {
    return false;
  }

  TArray<FFormatInfo_Import*> arr{};
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

  // データの要素数チェック
  check((arr.Num() - 1) == StaticCast<int32>(dataCnt));

  TArray<ESmithFormatType> typeSrcData;
  for (int32 i = 1; i < arr.Num(); ++i)
  {
    typeSrcData.Emplace(arr[i]->Type);
  }

  // TODO 生データで初期化する設計を見直し
  TSharedPtr<UE::Smith::Battle::FSmithCommandFormat> formatPtr = ::MakeShared<UE::Smith::Battle::FSmithCommandFormat>(typeSrcData.GetData(), dataCnt, formatRow, formatColumn);
  m_normalAttackFormatBuffer.Emplace(name, formatPtr);

  return true;
}

bool ASmithPlayerActor::enhanceImpl(int32 idx)
{
  if ((m_enhanceSystem == nullptr) || (m_commandMediator == nullptr) || (InventoryComponent == nullptr))
  {
    return false;
  }

  const FString upgradeInventoryName = TEXT("UpgradeMaterial");
  UObject* material = InventoryComponent->Get(upgradeInventoryName, idx);
  if (material == nullptr)
  {
    return false;
  }

  IParamAbsorbable* absorbItem = Cast<IParamAbsorbable>(material);
  if (absorbItem == nullptr)
  {
    return false;
  }

  CloseMenu();

  m_enhanceSystem->Enhance(Weapon, absorbItem);
  InventoryComponent->Remove(upgradeInventoryName, idx);
  m_commandMediator->SendIdleCommand(this);

  return true;
}

void ASmithPlayerActor::OnAttack(const FBattleResult& Result)
{
  if (Result.Damage <= 0)
  {
    return;
  }

  if (Result.DamageFrom != EDirection::Invalid)
  {
    const EDirection newDir = StaticCast<EDirection>((StaticCast<uint8>(Result.DamageFrom) + 4u) % StaticCast<uint8>(EDirection::DirectionCount));
    changeForwardImpl(newDir);
  }

  m_bIsDamaged = true;
  m_bCanReceiveInput = false;

  m_currentHealth -= Result.Damage;

  if (!IsDefeated())
  {
    if (AnimationComponent != nullptr)
    {
      AnimationComponent->SwitchAnimState(TEXT("Damaged"));
    }
  }

  notifyHealthChanged();
}

bool ASmithPlayerActor::IsDefeated() const
{
  return m_currentHealth <= 0;
}

void ASmithPlayerActor::OnDefeated()
{
  m_currentHealth = 0;
  m_bCanReceiveInput = false;

  if (OnDead.IsBound())
  {
    OnDead.Broadcast();
  }

  if (AnimationComponent != nullptr)
  {
    AnimationComponent->SwitchAnimState(TEXT("Dead"));
  }
    
  notifyHealthChanged();
}

void ASmithPlayerActor::OnTriggerEvent(USmithNextLevelEvent* event)
{
  check(event != nullptr);

  event->RaiseEvent();
  m_actorFaceDir = EDirection::South;
  SetActorRotation(FRotator{0.0, 180.0, 0.0});
}

// TODO Refactoring
void ASmithPlayerActor::OnTriggerEvent(USmithPickUpItemEvent* event)
{
  check(event != nullptr);

  if (InventoryComponent == nullptr)
  {
    return;
  }

  const FString itemType = event->GetPickUpItemType();
  const bool success = (InventoryComponent->ContainsCategory(itemType)) && (!InventoryComponent->IsReachCapacity(itemType));

  if (success)
  {
    IPickable* pickable = event->GetPickable();
    if (IS_UINTERFACE_VALID(pickable))
    {
      InventoryComponent->Insert(pickable->GetPickType(), pickable->_getUObject());
      if (OnHerbValueChanged.IsBound())
      {
        OnHerbValueChanged.Broadcast(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
      }

      event->RaiseEvent();
    }
  }

  // TODO
  if (m_logSender != nullptr)
  {
    m_logSender->SendInteractEventLog(this, event, success);
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
  const int32 recovery = StaticCast<int32>(StaticCast<double>(m_maxHealth) * recoveryPercentage);

  m_currentHealth += recovery;
  if (m_currentHealth > m_maxHealth)
  {
    m_currentHealth = m_maxHealth;
  }

  notifyHealthChanged();
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
  outName = NAME_None;

  static const TMap<uint8, FName> ANIM_STATE_NAME =
  {
    {SMITH_ANIM_IDLE, TEXT("Idle")},
    {SMITH_ANIM_WALK, TEXT("Walk")},
    {SMITH_ANIM_ATTACK, TEXT("Attack")},
    {SMITH_ANIM_DAMAGED, TEXT("Damaged")},
    {SMITH_ANIM_DEAD, TEXT("Dead")},
  };

  if (ANIM_STATE_NAME.Contains(animationState))
  {
    outName = ANIM_STATE_NAME[animationState];
  }
}

EDirection ASmithPlayerActor::GetCameraDirection() const
{
  return m_camDir;
}

#if WITH_EDITOR
void ASmithPlayerActor::SelfDamage_Debug(int32 damage)
{
  OnAttack(FBattleResult{damage, EDirection::Invalid});
}
#endif

void ASmithPlayerActor::updateParam(FParams upgradeParam)
{
  if (upgradeParam.HP != 0)
  {
    const float curtPercentage = StaticCast<float>(m_currentHealth) / StaticCast<float>(m_maxHealth);

    m_maxHealth += upgradeParam.HP;
    m_currentHealth = FMath::CeilToInt32(StaticCast<float>(m_maxHealth) * curtPercentage);

    notifyHealthChanged();
  }
}

bool ASmithPlayerActor::CanReceiveInputEvent() const
{
  return m_bCanReceiveInput && IsCommandSendable();
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
  if (m_turnCnt % 3 == 0)
  {
    if (m_currentHealth < m_maxHealth)
    {
      ++m_currentHealth;
      notifyHealthChanged();
    }
  }
}

// TODO
UTexture2D* ASmithPlayerActor::GetMinimapDisplayTexture_Implementation()
{
  return MinimapTexture;
}

void ASmithPlayerActor::OnAttackExecuted()
{
  int32 ran = FMath::RandRange(1, 2);
  FString atkSE = TEXT("Player_Slash_") + FString::FromInt(ran);
  AudioKit::PlaySE(atkSE);
}