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

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// TurnSystem Module
class IBattleCommand;

// SmithActor Module
struct AttackHandle;
enum class EDirection : uint8;

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

#pragma endregion Interfaces Override
// end of Interfaces Override

// Private Functions
#pragma region Private Functions
private:
	void moveImpl(EDirection);
	void attackImpl();
	void changeFwdImpl(EDir_Test);
	void updateCamImpl(EDir_Test);
	bool registerAttackFormat(const FString&, const UDataTable*);

private:
	// Input bind Functions
	void Move_Input(const FInputActionValue&);
	void Attack_Input(const FInputActionValue&);
	void Look_Input(const FInputActionValue&);
	void Debug_SelfDamage_Input(const FInputActionValue&);
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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_springArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_cam;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithMoveComponent> m_moveComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithAttackComponent> m_atkComponent;

	// Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MappingCtx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithEnhancedInput, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DebugAction;

	// Attack Format
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;

	TMap<FString,TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_normalAttackFormatBuffer;

	// TODO 仲介でシステムとやり取りする
	TWeakInterfacePtr<ICommandMediator> m_commandMediator;
#pragma endregion UProperties
// end of UProperties
//---------------------------------------
/*
            プライベートプロパティ
*/
//---------------------------------------
// Private Properties
#pragma region Private Properties
private:
	int32 m_hp;
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