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
class IEnhanceSystem;
class ISmithLogSender;
class UCameraComponent;
class USmithAnimationComponent;
class USmithInventoryComponent;
class USmithWeapon;
class USpringArmComponent;

struct FParams;
struct FSmithWeaponInfoHandle;

enum class EDirection : uint8;

namespace UE::Smith
{
  namespace Battle
  {
    class FSmithCommandFormat;
  }
}

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

//---Begin of AActor Interface
protected:

  void BeginPlay() override final;
  void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

public:	

  void Tick(float DeltaTime) override final;
//---End of AActor Interface

//---------------------------------------
/*
        パブリック関数(インターフェース)
*/
//---------------------------------------
//---Begin of IAttackable Interface
public:

  void OnAttack(const FBattleResult& Handle) override final;
  bool IsDefeated() const override final;
  void OnDefeated() override final;
  FBattleDefenseParamHandle GetDefenseParam() const override final;
//---End of IAttackable Interface

//---Begin of IEventTriggerable Interface
public:

  void OnTriggerEvent(USmithNextLevelEvent* Event) override final;
  void OnTriggerEvent(USmithPickUpItemEvent* Event) override final;
//---End of IEventTriggerable Interface

//---Begin of ICanCommandMediate Interface
public:

  void SetCommandMediator(ICommandMediator*) override final;
//---End of ICanCommandMediate Interface

//---Begin of ICanUseEnhanceSystem Interface
public:

  void SetEnhanceSystem(IEnhanceSystem*);
//---End of ICanUseEnhanceSystem Interface

//---Begin of ISmithAnimator Interface
public:

  void SwitchAnimation(uint8 animationState) override final;
  void SwitchAnimationDelay(uint8 animationState, float delay) override final;
  void UpdateAnimation(float deltaTime) override final;
  bool IsAnimationFinish() const override final;
//---End of ISmithAnimator Interface

private:
  /// @brief                  アニメーションステートをFNameに変換
  /// @param animationState   アニメーションステート(基本UE::Smith::EAnimationState使用)
  /// @param outName          アニメーションモンタージュセクション名前
  void convertAnimState(uint8 animationState, FName& outName);

//---Begin of IItemUseable Interface
public:

  void UseItem(USmithHPItem*);
//---End of IItemUseable Interface

//---Begin of IMinimapDisplayable Interface
public:

  UTexture2D* GetMinimapDisplayTexture_Implementation() override final;
//---End of IMinimapDisplayable Interface

public:
  
  /// 
  /// @brief        移動する 
  /// @param        移動入力方向
  /// 
  void Move(const FVector2D& InputDirection);
  ///
  /// @brief        攻撃する
  ///
  void Attack();
  ///
  /// @brief        向きを変える
  /// @param        向き変更入力方向
  ///
  void ChangeForward(const FVector2D& InputDirection);
  ///
  /// @brief        メニューを開く
  ///
  void OpenMenu();
  ///
  /// @brief        メニューを閉じる
  ///
  void CloseMenu();
  ///
  /// @brief                  次のメニューアイテムを選択
  /// @param SelectDirection  選択入力
  ///
  void SelectNextMenuItem(float SelectDirection);
  ///
  /// @brief        今選択しているメニューの項目と交互する
  ///
  void InteractMenu();
  ///
  /// @brief        HP回復
  ///
  void RecoverHealth();
  ///
  /// @brief        斜め移動縛り状態を設定
  ///
  void SetOnlyReceiveDiagonalInput(bool bValue);
  ///
  /// @brief        ModelのデータでViewの状態と同期する
  ///
  void SyncronizePlayerView();
  
  #if WITH_EDITOR
  void SelfDamage_Debug(int32);
  #endif

public:

  /// TODO
  /// @brief ゲームクリアした時呼び出されるコールバック
  void OnGameClear();
  
private:

  /// 
  /// @brief                  攻撃フォーマットを登録
  /// @param FormatName       フォーマット名前
  /// @param FormatDataTable　フォーマットデータテーブル
  ///
  void registerAttackFormat(const FString& FormatName, const UDataTable* FormatDataTable);
  ///
  /// @brief                  強化実装部分
  /// @param Idx              今選択しているアイテムのインデックス
  /// @return                 true->強化成功 false->強化失敗
  ///
  bool enhanceImpl(int32 Idx);
  ///
  /// @brief                  プレイヤー向きを変える実装部分
  /// @param NewDirection     新しい向き
  ///
  void changeForwardImpl(EDirection NewDirection);
  ///
  /// @brief                  パラメーター更新 
  /// @param                  強化した分のパラメーター
  ///
  void onEquipmentUpgrade(FParams upgradeParam);
  ///
  /// @brief                  ターン経過によるHP回復
  ///
  void turnPassRecover();

  ///
  /// @brief                  HP変更通知を送る
  ///
  void notifyHealthChanged() const;

//---------------------------------------
/*
              uproperty 宣言
*/
//---------------------------------------
private:
  // カメラアーム
  UPROPERTY(VisibleAnywhere, Category = Player)
  TObjectPtr<USpringArmComponent> CameraBoom;
  // カメラコンポーネント
  UPROPERTY(VisibleAnywhere, Category = Player)
  TObjectPtr<UCameraComponent> Camera;
  // インベントリコンポーネント
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USmithInventoryComponent> InventoryComponent;
  // アニメーションコンポーネント
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USmithAnimationComponent> AnimationComponent;
  // フォーマット（BPで設定）
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
  TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;
  // 武器
  UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
  TObjectPtr<USmithWeapon> Weapon;
  // ミニマップ表示テクスチャ
  UPROPERTY(EditAnywhere)
  TObjectPtr<UTexture2D> MinimapTexture;

private:

  // フォーマットバッファ
  TMap<FString,TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_normalAttackFormatBuffer;
  // コマンドを送る時に使うファサード
  TWeakInterfacePtr<ICommandMediator> m_commandMediator;        
  // 強化システム
  TWeakInterfacePtr<IEnhanceSystem> m_enhanceSystem;            
  // ログを送るインターフェース
  TWeakInterfacePtr<ISmithLogSender> m_logSender;               

public:

  // モデル更新イベント
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
private:

  int32 m_currentHealth;
  int32 m_maxHealth;
  int32 m_turnCnt;
  EDirection m_camDir;            // カメラ向き
  EDirection m_actorFaceDir;      // プレイヤー向き

  uint8 m_bIsInMenu : 1;
  uint8 m_bCanReceiveInput : 1;
  uint8 m_bIsDamaged : 1;
  uint8 m_bOnlyMoveDiagonal : 1;
};