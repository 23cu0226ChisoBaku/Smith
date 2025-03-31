// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithPlayerActor.h

Author : MAI ZHICONG

Description : プレイヤークラス（Pawn）

Update History: 2024/12/12 開始日
                ..../12/20 アルファ完成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "GameFramework/Pawn.h"

#include "Direction.h"
#include "IAttackable.h"
#include "IAttackCauser.h"
#include "ICanCommandMediate.h"
#include "ICanUseEnhanceSystem.h"
#include "IEventTriggerable.h"
#include "IItemUseable.h"
#include "IMinimapDisplayable.h"
#include "ISmithAnimator.h"
#include "ITurnManageable.h"
#include "UObject/WeakInterfacePtr.h"

#include "SmithPlayerActor.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
// Forward Declaration
#pragma region Forward Declaration
// Unreal Component
class USpringArmComponent;
class UCameraComponent;
class USmithInventoryComponent;
class USmithAnimationComponent;

class USmithBattleLogWorldSubsystem;

// SmithActor Module
struct AttackHandle;
enum class EDirection : uint8;

class IEnhanceSystem;
class USmithWeapon;
struct FParams;
struct FSmithWeaponInfoHandle;

namespace UE::Smith
{
  namespace Battle
  {
    class FSmithCommandFormat;
  }
}
#pragma endregion Forward Declaration
// end of Forward Declaration

DECLARE_DELEGATE_TwoParams(FInitializeMenuEvent, const FSmithWeaponInfoHandle&, const TArray<UObject*>&);

///
/// @brief プレイヤークラス
/// TODO インターフェースを減らす
///
UCLASS()
class SMITH_API ASmithPlayerActor final:  public APawn, public ITurnManageable, 
                                          public IAttackable, public IAttackCauser,
                                          public IEventTriggerable, public ICanCommandMediate,
                                          public ICanUseEnhanceSystem, public ISmithAnimator,
                                          public IItemUseable, public IMinimapDisplayable
{
  GENERATED_BODY()

public:

  ASmithPlayerActor();

#pragma region AActor Interfaces
//---Begin of AActor Interfaces
protected:

  void BeginPlay() override final;
  void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

public:	

  void Tick(float DeltaTime) override final;

//---End of AActor Interfaces
#pragma endregion AActor Interfaces

//---------------------------------------
/*
        パブリック関数(インターフェース)
*/
//---------------------------------------
// Interfaces Override
#pragma region Interfaces Override

  // IAttackable (Smith Module)
  #pragma region IAttackable
  public:

    void OnAttack(const AttackHandle& Handle) override final;
    bool IsDefeated() const override final;
    void OnDefeated() override final;

  #pragma endregion IAttackable
  // end of IAttackable

  // ICanCommandMediate (Smith Module)
  #pragma region ICanCommandMediate
  public:

    void SetCommandMediator(ICommandMediator*) override final;

  #pragma endregion ICanCommandMediate
  // end of ICanCommandMediate

  public:

    void OnTriggerEvent(USmithNextLevelEvent*) override final;
    void OnTriggerEvent(USmithPickUpItemEvent*) override final;

  public:

    void SwitchAnimation(uint8 animationState) override final;
    void SwitchAnimationDelay(uint8 animationState, float delay) override final;
    void UpdateAnimation(float deltaTime) override final;
    bool IsAnimationFinish() const override final;

  public:

    void UseItem(USmithHPItem*);
    void SetEnhanceSystem(IEnhanceSystem*);
    FBattleDefenseParamHandle GetDefenseParam() const override;

  public:
    UTexture2D* GetMinimapDisplayTexture_Implementation() override final;

  public:
    void OnAttackExecuted() override;

  private:
    void convertAnimState(uint8 animationState, FName& outName);

#pragma endregion Interfaces Override
// end of Interfaces Override

public:

  EDirection GetCameraDirection() const;
  bool CanReceiveInputEvent() const;

public:

  void Move(EDirection);
  void Attack();
  void ChangeForward(EDirection);
  void OpenMenu();
  void CloseMenu();
  void SelectNextMenuItem(float SelectDirection);
  void InteractMenu();
  void RecoverHealth();
  bool registerAttackFormat(const FString&, const UDataTable*);

  #if WITH_EDITOR
    void SelfDamage_Debug(int32);
  #endif

// Private Functions
#pragma region Private Functions
private:

  bool enhanceImpl(int32 idx);
  void updateParam(FParams upgradeParam);
  void changeForwardImpl(EDirection);
  void turnPassRecover();

  void notifyHealthChanged() const
  {
    if (OnHealthChanged.IsBound())
    {
      OnHealthChanged.Broadcast(m_currentHealth, m_maxHealth);
    }
  }
#pragma endregion Private Functions

public:

  void OnGameClear();
// end of Private Functions

//---------------------------------------
/*
              uproperty 宣言
*/
//---------------------------------------
// UProperties
#pragma region UProperties
private:
  // Components
  UPROPERTY(VisibleAnywhere, Category = Player)
  TObjectPtr<USpringArmComponent> CameraBoom;
  
  UPROPERTY(VisibleAnywhere, Category = Player)
  TObjectPtr<UCameraComponent> Camera;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USmithInventoryComponent> InventoryComponent;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USmithAnimationComponent> AnimationComponent;
  
  /** 攻撃フォーマット */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
  TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;

  TMap<FString,TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_normalAttackFormatBuffer;

  // TODO　装備を増やすこと
  UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
  TObjectPtr<USmithWeapon> Weapon;

  TWeakInterfacePtr<ICommandMediator> m_commandMediator;        // コマンドを送る時に使う仲介

  TWeakInterfacePtr<IEnhanceSystem> m_enhanceSystem;          // 強化

  UPROPERTY()
  TObjectPtr<USmithBattleLogWorldSubsystem> m_logSystem;

  // TODO ミニマップの表示テクスチャをまとめて管理するように設計し直す
  UPROPERTY(EditAnywhere)
  TObjectPtr<UTexture2D> MinimapTexture;

#pragma endregion UProperties
// end of UProperties

public:

  TMulticastDelegate<void()> OnDead;

  TMulticastDelegate<void(int32)> OnHerbValueChanged;
  TMulticastDelegate<void(int32, int32)> OnHealthChanged;
  TMulticastDelegate<void()> OnEnhanceMenuOpened;
  TMulticastDelegate<void()> OnEnhanceMenuClosed;
  TMulticastDelegate<void(float)> OnMenuItemChangeFocus;

  TDelegate<int32()> OnItemSelected;
  FInitializeMenuEvent OnEnhanceMenuInitialized;

//---------------------------------------
/*
            プライベートプロパティ
*/
//---------------------------------------
// Private Properties
#pragma region Private Properties
private:

  int32 m_currentHealth;
  int32 m_maxHealth;
  int32 m_turnCnt;
  EDirection m_camDir;            // カメラ向き
  EDirection m_actorFaceDir;      // プレイヤー向き

  uint8 m_bIsInMenu : 1;
  uint8 m_bCanReceiveInput : 1;
  uint8 m_bIsDamaged : 1;

#pragma endregion Private Properties
// end of Private Properties
};