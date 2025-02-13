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

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/WeakInterfacePtr.h"
#include "Direction.h"
#include "ITurnManageable.h"
#include "IAttackable.h"
#include "ICanCommandMediate.h"
#include "ICanSetOnMap.h"
#include "IEventTriggerable.h"
#include "ICanPick.h"
#include "ICanUseEnhanceSystem.h"
#include "ISmithAnimator.h"
#include "ISmithBattleLogger.h"
#include "IItemUseable.h"
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
class USmithMoveComponent;
class USmithAttackComponent;
class USmithInventoryComponent;
class USmithAnimationComponent;
class UHPUIComponent;
class USmithUpgradeInteractiveComponent;

// TODO インターフェースにする
class USmithBattleLogWorldSubsystem;

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// TurnSystem Module
class IBattleCommand;

// SmithActor Module
struct AttackHandle;
enum class EDirection : uint8;

class IEnhanceSystem;
class USmithWeapon;
class UHerbWidget;
struct FParams;

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
	}
}
#pragma endregion Forward Declaration
// end of Forward Declaration

///
/// @brief プレイヤークラス
///
UCLASS()
class SMITH_API ASmithPlayerActor final: public APawn, public ITurnManageable
																			 , public IAttackable, public ICanCommandMediate
																			 , public ICanSetOnMap, public IEventTriggerable
																			 , public ICanUseEnhanceSystem, public ISmithAnimator
																			 , public ISmithBattleLogger, public IItemUseable
{
	GENERATED_BODY()

//---------------------------------------
/*
                  ctor
*/
//---------------------------------------
public:
	ASmithPlayerActor();

//---------------------------------------
/*
           アクター ライフサイクル
*/
//---------------------------------------
// Lifecycle
#pragma region Lifecycle
protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

public:	
	void Tick(float DeltaTime) override final;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override final;
#pragma endregion Lifecycle
// end of Lifecycle

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
		void OnAttack(AttackHandle&&) override final;
	#pragma endregion IAttackable
	// end of IAttackable

	// ICanCommandMediate (Smith Module)
	#pragma region ICanCommandMediate
	public:
		void SetCommandMediator(ICommandMediator*) override final;
	#pragma endregion ICanCommandMediate
	// end of ICanCommandMediate

	public:
		uint8 GetOnMapSizeX() const override final;
		uint8 GetOnMapSizeY() const override final;
		EMapObjType GetType() const override final;

	public:
		void OnTriggerEvent(USmithNextLevelEvent*) override final;
		void OnTriggerEvent(USmithPickUpItemEvent*) override final;

	public:
		void SwitchAnimation(uint8 animationState) override final;
		void SwitchAnimationDelay(uint8 animationState, float delay) override final;
		void UpdateAnimation(float deltaTime) override final;
		bool IsAnimationFinish() const override final;

	public:
		FString GetName_Log() const override;
		EBattleLogType GetType_Log() const override;

	public:
		void UseItem(USmithHPItem*);

		void SetEnhanceSystem(IEnhanceSystem*);

		FBattleDefenseParamHandle GetDefenseParam() const override;
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
	void ChangeCameraDirection(EDirection, bool bIsClockwise);
	void OpenMenu();
	void CloseMenu();
	void SelectNextMenuItem(float direction);
	bool InteractMenu();
	void RecoverHealth();
	bool registerAttackFormat(const FString&, const UDataTable*);
	void SelfDamage_Debug(int32);

// Private Functions
#pragma region Private Functions
private:
	bool enhanceImpl(int32 idx);
	void updateCamera(float deltaTime);
	void updateParam(FParams upgradeParam);
	void changeForwardImpl(EDirection);
	void turnPassRecover();
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
	/** カメラアーム */
	UPROPERTY(VisibleAnywhere, Category = Player)
	TObjectPtr<USpringArmComponent> CameraBoom;
	/** カメラ */
	UPROPERTY(VisibleAnywhere, Category = Player)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithMoveComponent> MoveComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithAttackComponent> AttackComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithInventoryComponent> InventoryComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithAnimationComponent> AnimationComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHPUIComponent> HPComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithUpgradeInteractiveComponent> UpgradeInteractiveComponent;
	
	/** 攻撃フォーマット */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;

	// TODO
	UPROPERTY(EditAnywhere, Instanced, Category = Weapon)
	TObjectPtr<USmithWeapon> Weapon;

	// TODO
	UPROPERTY()
	TObjectPtr<UHerbWidget> m_herbUI;
	UPROPERTY(EditAnywhere, Category = RecoveryItem)
	TSubclassOf<UHerbWidget> HerbUISub;

	TMap<FString,TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_normalAttackFormatBuffer;

	// コマンドを送る時に使う仲介
	TWeakInterfacePtr<ICommandMediator> m_commandMediator;
	// 強化
	TWeakInterfacePtr<IEnhanceSystem> m_enhanceSystem;

	// TODO インターフェースにしてより柔軟性のいい設計
	UPROPERTY()
	TObjectPtr<USmithBattleLogWorldSubsystem> m_logSystem;


#pragma endregion UProperties
// end of UProperties

public:
	TMulticastDelegate<void()> OnDead;
	TMulticastDelegate<void()> OnStartCameraRotation;
	TMulticastDelegate<void()> OnFinishCameraRotation;
//---------------------------------------
/*
            プライベートプロパティ
*/
//---------------------------------------
// Private Properties
#pragma region Private Properties
private:
	int32 m_curtHP;
	int32 m_maxHP;
	float m_rotateSpeed;
	int32 m_rotatingDirection;
	int32 m_turnCnt;
	EDirection m_camDir;
	EDirection m_actorFaceDir;
	uint8 m_bCanMove : 1;
	uint8 m_bCanAttack : 1;
	uint8 m_bRotatingCamera : 1;
	uint8 m_bIsInMenu : 1;
	uint8 m_bCanReceiveInput : 1;
	uint8 m_bIsDamaged : 1;

#pragma endregion Private Properties
// end of Private Properties
};