// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerActor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SmithInventoryComponent.h"
#include "SmithAnimationComponent.h"

#include "AttackHandle.h"
#include "BattleParamHandle.h"
#include "SmithCommandFormat.h"
#include "FormatInfo_Import.h"

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

#include "SmithModelHelperFunctionLibrary.h"

#include "MLibrary.h"

using namespace MLibrary::UE::Audio;

namespace SmithPlayerActor::Private
{
  constexpr double ANGLE_PER_DIRECTION = 360.0 / (double)EDirection::DirectionCount;
  constexpr double MOVE_DEAD_ZONE_SCALAR = 0.5;
  constexpr uint8 DIRECTION_COUNT = (uint8)EDirection::DirectionCount;
  // ベクトルを方向列挙に変換する(X,Yだけ,Zは無視)
	EDirection VectorDirToEDir(const FVector& direction , bool bOnlyDiagonal = false)
	{
		const double dot = direction.Dot(FVector::ForwardVector);
		const FVector cross = direction.Cross(FVector::ForwardVector);
		const double angle = FMath::RadiansToDegrees(acos(dot));
		// (1,0,0)(※EDirection::Northを表すベクトル)から時計回りに回転しdirectionまで回転した角度を計算
		const double angleClockwise =  cross.Z > 0.0 ? - angle : angle; 

    EDirectionPolicy strategy = EDirectionPolicy::Ordinal;
    if (bOnlyDiagonal)
    {
      strategy = EDirectionPolicy::Diagonal;
    }

		return FSmithModelHelperFunctionLibrary::GetDirectionOfDegree(angleClockwise, strategy);
	}

  EDirection CalculateDirectionRelativeCamera(EDirection cameraDirection, const FVector2D& inputValue, bool bOnlyDiagonal = false)
  {
    const FVector2D normalizedInput = inputValue.GetSafeNormal();
    if (normalizedInput.IsNearlyZero())
    {
      return EDirection::Invalid;
    }

      // カメラの角度で回転ベクトルを計算する
    const double cameraAngle = FMath::DegreesToRadians(StaticCast<double>(cameraDirection) * ANGLE_PER_DIRECTION);
    double directionX = normalizedInput.Y * cos(cameraAngle) - normalizedInput.X * sin(cameraAngle);
    double directionY = normalizedInput.Y * sin(cameraAngle) + normalizedInput.X * cos(cameraAngle);

    // ゼロ補正
    if (FMath::IsNearlyZero(directionX))
    {
      directionX = 0.0;
    }

    if (FMath::IsNearlyZero(directionY))
    {
      directionY = 0.0;
    }

    const EDirection newDirection = VectorDirToEDir(FVector{directionX, directionY, 0.0}, bOnlyDiagonal);
    return newDirection;
  }
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
  , m_bOnlyMoveDiagonal(false)
{

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
  check(InventoryComponent != nullptr);

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
  
    Weapon->OnParamUpdated.AddUObject(this, &ASmithPlayerActor::onEquipmentUpgrade);
  }

  // HP初期化
  {
    m_maxHealth += Weapon->GetParam().HP;
    m_currentHealth = m_maxHealth;
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
  }

  // 初期状態の向き
  m_actorFaceDir = EDirection::South;
  SetActorRotation(FRotator{0.0, 180.0, 0.0});

  // ターン優先順位を設定
  SetTurnPriority(ETurnPriority::PlayerSelf);
  OnTurnPass.AddUObject(this, &ASmithPlayerActor::turnPassRecover);

  #if WITH_EDITOR
    // TODO パッケージした後にPlayerControllerのBeginPlayの実行順番がプレイヤーの後にくるため、
    // PlayerController::BeginPlay()で呼び出される
    // エディタでプレイヤーポーンのBeginPlayの実行順番がPlayerControllerの後にくる。 <- 謎 
    SyncronizePlayerView();
  #endif
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (Weapon != nullptr)
  {
    Weapon->OnParamUpdated.Clear();
  }
}

void ASmithPlayerActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  // HPがなくなったらTickを停める
  if (m_currentHealth <= 0)
  {
    SetActorTickEnabled(false);
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

///
/// @brief 攻撃を受けた時に呼び出されるコールバック
///
void ASmithPlayerActor::OnAttack(const FBattleResult& Result)
{
  if (Result.Damage <= 0)
  {
    return;
  }

  if (Result.DamageFrom != EDirection::Invalid)
  {
    // 攻撃をくらった方向に向く
    const EDirection newDir = StaticCast<EDirection>((StaticCast<uint8>(Result.DamageFrom) + 4u) % StaticCast<uint8>(EDirection::DirectionCount));
    changeForwardImpl(newDir);
  }

  m_bIsDamaged = true;
  m_bCanReceiveInput = false;
  m_currentHealth -= Result.Damage;

  // ダメージをくらったアニメーション
  if (!IsDefeated())
  {
    if (AnimationComponent != nullptr)
    {
      FName animName = NAME_None;
      convertAnimState(UE::Smith::SMITH_ANIM_DAMAGED, animName);
      AnimationComponent->SwitchAnimState(animName);
    }
  }

  notifyHealthChanged();
}

///
/// @brief  倒されたかを調べる
///
bool ASmithPlayerActor::IsDefeated() const
{
  return m_currentHealth <= 0;
}

///
/// @brief 倒されたら呼び出されるコールバック
///
void ASmithPlayerActor::OnDefeated()
{
  m_currentHealth = 0;
  m_bCanReceiveInput = false;

  if (OnDead.IsBound())
  {
    OnDead.Broadcast();
  }

  // 死亡アニメーション
  if (AnimationComponent != nullptr)
  {
    FName animName = NAME_None;
    convertAnimState(UE::Smith::SMITH_ANIM_DEAD, animName);
    AnimationComponent->SwitchAnimState(animName);
  }
    
  notifyHealthChanged();
}

FBattleDefenseParamHandle ASmithPlayerActor::GetDefenseParam() const
{
  if (Weapon == nullptr)
  {
    return IAttackable::GetDefenseParam();		
  }

  FBattleDefenseParamHandle handle;
  handle.DefensePoint = Weapon->GetParam().DEF;
  handle.Level = Weapon->GetLevel();

  return handle;
}

///
/// @brief 次の階層へいくイベントを作動した時に呼び出されるコールバック
///
void ASmithPlayerActor::OnTriggerEvent(USmithNextLevelEvent* event)
{
  check(event != nullptr);

  event->RaiseEvent();
  m_actorFaceDir = EDirection::South;
  SetActorRotation(FRotator{0.0, 180.0, 0.0});
}

///
/// @brief 拾うイベントを作動した時に呼び出されるコールバック
///
void ASmithPlayerActor::OnTriggerEvent(USmithPickUpItemEvent* event)
{
  check(event != nullptr);
  if (InventoryComponent == nullptr)
  {
    return;
  }

  // 拾うアイテムをインベントリに入れる
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
    }
  }
  
  event->RaiseEvent();

  if (m_logSender != nullptr)
  {
    m_logSender->SendInteractEventLog(this, event, success);
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

///
/// @brief  アニメーション状態を切り替える(遅延なし)
///
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

///
/// @brief  アニメーション状態を切り替える(遅延あり)
///
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

///
/// @brief  アニメーション状態を更新する
///
void ASmithPlayerActor::UpdateAnimation(float deltaTime)
{
  if (AnimationComponent == nullptr)
  {
    return;
  }

  AnimationComponent->UpdateAnim(deltaTime);
}

///
/// @brief  設定したアニメーション再生が終了したかを調べる
///
bool ASmithPlayerActor::IsAnimationFinish() const
{
  return (AnimationComponent != nullptr) ? AnimationComponent->IsCurrentAnimationFinish() : true;
}

///
/// @brief  ステートをアニメーションセクションの名前に変換
///
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

///
/// @brief  アイテムを使用する時に呼び出されるコールバック
///
void ASmithPlayerActor::UseItem(USmithHPItem* item)
{
  if (item == nullptr)
  {
    return;
  }

  // 回復割合で回復量を算出
  const double recoveryPercentage = item->GetRecoveryPercentage();
  const int32 recovery = StaticCast<int32>(StaticCast<double>(m_maxHealth) * recoveryPercentage);

  m_currentHealth += recovery;
  if (m_currentHealth > m_maxHealth)
  {
    m_currentHealth = m_maxHealth;
  }

  notifyHealthChanged();
}

UTexture2D* ASmithPlayerActor::GetMinimapDisplayTexture_Implementation()
{
  return MinimapTexture;
}

///
/// @brief  現在のパラメーターでGUIを更新
///
void ASmithPlayerActor::SyncronizePlayerView()
{
  notifyHealthChanged();
  if (InventoryComponent != nullptr)
  {
    if (OnHerbValueChanged.IsBound())
    {
      OnHerbValueChanged.Broadcast(InventoryComponent->GetQuantity(TEXT("ConsumeItem")));
    }
  }
}

///
/// @brief  移動処理
///
void ASmithPlayerActor::Move(const FVector2D& InputDirection)
{
  if (   !IsCommandSendable() 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  // 移動する前に向きを変える
  ChangeForward(InputDirection);

  // 移動コマンドを出す
  if (m_commandMediator.IsValid())
  {
    using namespace SmithPlayerActor::Private;
    const EDirection newDirection = CalculateDirectionRelativeCamera(m_camDir, InputDirection, m_bOnlyMoveDiagonal);

    bool success = m_commandMediator->SendMoveCommand(this, newDirection, 1);
    // 移動命令が失敗したとき待機アニメーションに戻る
    if (!success)
    {
      if (AnimationComponent != nullptr)
      {
        FName outName = NAME_None;
        convertAnimState(UE::Smith::SMITH_ANIM_IDLE, outName);
        AnimationComponent->SwitchAnimState(outName);
      }
    }
  }
}

///
/// @brief  攻撃処理
///
void ASmithPlayerActor::Attack()
{
  if (   !IsCommandSendable() 
      || !m_commandMediator.IsValid()
      || !m_bCanReceiveInput 
     )
  {
    return;
  }

  // プレイヤーの向きでFormatを選ぶ
  // 斜め（NorthEast:1/SouthEast:3/SouthWest:5/NorthWest:7）
  static const TMap<uint8, const FString> NORMAL_ATTACK_KEY_TABLE =
  {
    {0u, TEXT("Attack")},
    {1u, TEXT("AttackDiagonal")}
  };

  const FString attackKey = NORMAL_ATTACK_KEY_TABLE[StaticCast<uint8>(m_actorFaceDir) % 2];
  if (!m_normalAttackFormatBuffer.Contains(attackKey) || !m_normalAttackFormatBuffer[attackKey].IsValid())
  {
    return;
  }

  // 攻撃ハンドル作成
  FAttackHandle attackHandle{};
  attackHandle.AttackPower = Weapon->GetParam().ATK;
  attackHandle.Level = Weapon->GetLevel();
  attackHandle.MotionValue = 1.0;

  m_commandMediator->SendAttackCommand(this, m_actorFaceDir, *m_normalAttackFormatBuffer[attackKey], attackHandle);

}

///
/// @brief  方向変換処理
///
void ASmithPlayerActor::ChangeForward(const FVector2D& InputDirection)
{
  if (   !IsCommandSendable() 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  using namespace SmithPlayerActor::Private;
  const EDirection newDirection = CalculateDirectionRelativeCamera(m_camDir, InputDirection, m_bOnlyMoveDiagonal);

  changeForwardImpl(newDirection);
}

///
/// @brief  メニューを開く
///
void ASmithPlayerActor::OpenMenu()
{
  if (   !IsCommandSendable() 
      || m_bIsInMenu 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  m_bIsInMenu = true;

  if ((Weapon != nullptr) && (InventoryComponent != nullptr))
  {
    // 強化素材を全部取得し、強化画面に渡す
    TArray<UObject*> itemList{};
    InventoryComponent->GetAll(TEXT("UpgradeMaterial"), itemList);
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

///
/// @brief  メニューを閉じる
///
void ASmithPlayerActor::CloseMenu()
{
  if (   !IsCommandSendable() 
      || !m_bIsInMenu 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  m_bIsInMenu = false;
  if (OnEnhanceMenuClosed.IsBound())
  {
    OnEnhanceMenuClosed.Broadcast();
  }
}

///
/// @brief  次のメニューアイテムを選択する
///
void ASmithPlayerActor::SelectNextMenuItem(float SelectDirection)
{
  if (   !IsCommandSendable() 
      || !m_bIsInMenu 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  if (OnMenuItemChangeFocus.IsBound())
  {
    OnMenuItemChangeFocus.Broadcast(SelectDirection);
  }
}

///
/// @brief  今選択しているメニューアイテムを操作する
///
void ASmithPlayerActor::InteractMenu()
{
  if (   !IsCommandSendable() 
      || !m_bIsInMenu 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  if (OnItemSelected.IsBound())
  {
    // 今選んでいるメニュー項目のIdxを取得
    const int32 selectedItemIdx = OnItemSelected.Execute();
    if (selectedItemIdx != -1)
    {
      // 強化した後にメニューを閉じる
      bool success = enhanceImpl(selectedItemIdx);
      if (success)
      {
        CloseMenu();
      }
    }
  }
}

///
/// @brief HPを回復
///
void ASmithPlayerActor::RecoverHealth()
{
  if (   !IsCommandSendable() 
      || !m_bCanReceiveInput
     )
  {
    return;
  }

  if (   !m_commandMediator.IsValid() 
      || (InventoryComponent == nullptr)
     )
  {
    return;
  }

  // インベントリの先頭回復アイテムを取得し、回復する
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

  // 使用成功したら一ターン経過
  m_commandMediator->SendIdleCommand(this);
  
}

///
/// @brief 斜め移動の状態を設定する
///
void ASmithPlayerActor::SetOnlyReceiveDiagonalInput(bool bValue)
{
  m_bOnlyMoveDiagonal = bValue;
}

///
/// @brief ゲームクリアした時に呼び出されるコールバック
///
void ASmithPlayerActor::OnGameClear()
{
  m_bCanReceiveInput = false;
}

///
/// @brief 攻撃フォーマットを登録する
///
void ASmithPlayerActor::registerAttackFormat(const FString& name, const UDataTable* formatTable)
{
  check(formatTable != nullptr)
  if (m_normalAttackFormatBuffer.Contains(name))
  {
    return;
  }

  TArray<FFormatInfo_Import*> arr{};
  formatTable->GetAllRows<FFormatInfo_Import>("", arr);
  if (arr.Num() <= 1)
  {
    return;
  }

  TArray<FName> names = formatTable->GetRowNames();
  if (!names[0].IsEqual(TEXT("FormatInfo")))
  {
    return;
  }
  
  const uint8 formatRow = arr[0]->Row;
  const uint8 formatColumn = arr[0]->Column;
  const size_t dataCnt = formatRow * formatColumn;

  // データの要素数が合うかどうかをチェック
  check((arr.Num() - 1) == StaticCast<int32>(dataCnt));

  TArray<ESmithFormatType> typeSrcData{};
  for (int32 i = 1; i < arr.Num(); ++i)
  {
    typeSrcData.Emplace(arr[i]->Type);
  }

  // TODO 生データで初期化する設計を見直し
  TSharedPtr<UE::Smith::Battle::FSmithCommandFormat> formatPtr = ::MakeShared<UE::Smith::Battle::FSmithCommandFormat>(typeSrcData.GetData(), dataCnt, formatRow, formatColumn);
  m_normalAttackFormatBuffer.Emplace(name, formatPtr);
}

///
/// @brief 強化処理実装部分
///
bool ASmithPlayerActor::enhanceImpl(int32 idx)
{
  if (   !m_enhanceSystem.IsValid()
      || !m_commandMediator.IsValid() 
      || (InventoryComponent == nullptr)
     )
  {
    return false;
  }

  // 選択しているアイテムのインデックスを取得し、強化する
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

  m_enhanceSystem->Enhance(Weapon, absorbItem);
  InventoryComponent->Remove(upgradeInventoryName, idx);
  
  // 強化した後にメニューを閉じて、１ターンを待機
  CloseMenu();
  m_commandMediator->SendIdleCommand(this);

  return true;
}

///
/// @brief プレイヤーの向き変換実装部分
///
void ASmithPlayerActor::changeForwardImpl(EDirection NewDirection)
{
  using namespace SmithPlayerActor::Private;
  // プレイヤー向き更新
  if (m_actorFaceDir != NewDirection)
  {
    m_actorFaceDir = NewDirection;
    const double newYaw = StaticCast<double>(m_actorFaceDir) * ANGLE_PER_DIRECTION;
    SetActorRotation(FRotator{0.0, newYaw, 0.0});
  }
}

///
/// @brief 装備が強化されたら呼び出されるコールバック
///
void ASmithPlayerActor::onEquipmentUpgrade(FParams upgradeParam)
{
  // HPの変更があったら割合を維持して、現在のHPを更新する
  if (upgradeParam.HP != 0)
  {
    const float curtPercentage = StaticCast<float>(m_currentHealth) / StaticCast<float>(m_maxHealth);

    m_maxHealth += upgradeParam.HP;
    m_currentHealth = FMath::CeilToInt32(StaticCast<float>(m_maxHealth) * curtPercentage);

    notifyHealthChanged();
  }
}

///
/// @brief ターン経過した時の処理
///
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

///
/// @brief HP変更通知を送信
///
void ASmithPlayerActor::notifyHealthChanged() const
{
  if (OnHealthChanged.IsBound())
  {
    OnHealthChanged.Broadcast(m_currentHealth, m_maxHealth);
  }
}


#if WITH_EDITOR
void ASmithPlayerActor::SelfDamage_Debug(int32 damage)
{
  OnAttack(FBattleResult{damage, EDirection::Invalid});
}
#endif




