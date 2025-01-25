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
#include "ITurnManageable.h"
#include "IAttackable.h"
#include "ICanCommandMediate.h"
#include "ICanSetOnMap.h"
#include "IEventTriggerable.h"
#include "ICanPick.h"
#include "ICanUseEnhanceSystem.h"
#include "ISmithAnimator.h"
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

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// TurnSystem Module
class IBattleCommand;

// SmithActor Module
struct AttackHandle;
enum class EDirection : uint8;

class USmithWeapon;
class IEnhanceSystem;

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
																			 , public ICanUseEnhanceSystem, public ICanPick
																			 , public ISmithAnimator
{
	GENERATED_BODY()

// TODO Test用方向列挙
public:
	enum EDir_Test : uint8
	{
		North = 0,						// 上方向
		NorthEast = 1,				// 右上
		East = 2,							// 右
		SouthEast = 3,				// 右下
		South = 4,						// 下
		SouthWest = 5,				// 左下
		West = 6,							// 左
		NorthWest = 7,				// 左上

		DirectionCnt,					// 選べられる方向の数
	};

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
		void PickUpConsume(USmithConsumeItem*) override final;
		void PickUpMaterial(USmithUpgradeMaterial*) override final;

	public:
		void SwitchAnimation(uint8 animationState) override final;
		void SwitchAnimationDelay(uint8 animationState, float delay) override final;
		void UpdateAnimation(float deltaTime) override final;
		bool IsAnimationFinish() const override final;
	
	private:
		void convertAnimState(uint8 animationState, FName& outName, float& outDurationTime);
	public:
		void SetEnhanceSystem(IEnhanceSystem*);

#pragma endregion Interfaces Override
// end of Interfaces Override

// Private Functions
#pragma region Private Functions
private:
	void moveImpl(EDirection);
	void attackImpl();
	void changeFwdImpl(EDir_Test);
	void updateCamImpl(EDir_Test);
	void enhanceImpl();
	bool registerAttackFormat(const FString&, const UDataTable*);

private:
	// Input bind Functions
	void Move_Input(const FInputActionValue&);
	void Attack_Input(const FInputActionValue&);
	void Look_Input(const FInputActionValue&);
	void Debug_SelfDamage_Input(const FInputActionValue&);
	void Enhance_Input(const FInputActionValue&);
#pragma endregion Private Functions
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


	// Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MappingCtx;
	/** 移動インプットアクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	/** 攻撃インプットアクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;
	/** カメラ移動インプットアクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraAction;
	/** デバッグ専用！！ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DebugAction;
	/** 強化インプットアクション */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EnhanceAction;
	
	/** 攻撃フォーマット */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;

	// TODO
	UPROPERTY(EditAnywhere, Instanced, Category = Weapon)
	TObjectPtr<USmithWeapon> Weapon;


	TMap<FString,TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_normalAttackFormatBuffer;

	// コマンドを送る時に使う仲介
	TWeakInterfacePtr<ICommandMediator> m_commandMediator;
	// 強化
	TWeakInterfacePtr<IEnhanceSystem> m_enhanceSystem;
#pragma endregion UProperties
// end of UProperties

public:
	TDelegate<void()> OnDead;
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
	EDir_Test m_camDir;
	EDir_Test m_actorFaceDir;
	uint8 m_bCanMove : 1;
	uint8 m_bCanAttack : 1;
	uint8 m_bRotatingCamera : 1;
	

#pragma endregion Private Properties
// end of Private Properties
};