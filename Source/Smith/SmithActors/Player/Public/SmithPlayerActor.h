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
class EnhanceSystem;

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// TurnSystem Module
class IBattleCommand;

// SmithActor Module
struct AttackHandle;

namespace UE::Smith
{
	namespace Battle
	{
		enum class EMoveDirection : uint8;
	}
}
#pragma endregion Forward Declaration
// end of Forward Declaration

///
/// @brief プレイヤークラス
///
UCLASS()
class SMITH_API ASmithPlayerActor final : public APawn, public ITurnManageable, public IAttackable, public ICanCommandMediate, public ICanSetOnMap
{
	GENERATED_BODY()

	// TODO Test用方向列挙
public:
	enum EDir_Test : uint8
	{
		North = 0,		 // 上方向
		NorthEast = 1, // 右上
		East = 2,			 // 右
		SouthEast = 3, // 右下
		South = 4,		 // 下
		SouthWest = 5, // 左下
		West = 6,			 // 左
		NorthWest = 7, // 左上

		DirectionCnt, // 選べられる方向の数
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
	void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override final;
#pragma endregion Lifecycle
// end of Lifecycle

//---------------------------------------
/*
				パブリック関数(インターフェース)
*/
//---------------------------------------
// Interfaces Override
#pragma region Interfaces Override

// IAttackable (SmithActor Module)
#pragma region IAttackable
public:
	void OnAttack(AttackHandle &&) override final;
#pragma endregion IAttackable
// end of IAttackable

// ICanCommandMediate (SmithGod Module)
#pragma region ICanCommandMediate
public:
	void SetCommandMediator(ICommandMediator *) override final;
#pragma endregion ICanCommandMediate
	// end of ICanCommandMediate

public:
	uint8 GetOnMapSizeX() const override final;
	uint8 GetOnMapSizeY() const override final;

#pragma endregion Interfaces Override
// end of Interfaces Override

// Private Functions
#pragma region Private Functions
private:
	void moveImpl(UE::Smith::Battle::EMoveDirection);
	void attackImpl();
	void changeFwdImpl(EDir_Test);
	void updateCamImpl(EDir_Test);

	// TODO Temp function
	bool searchActorsInDirection(FVector, TArray<AActor *> &);

private:
	// Input bind Functions
	void Move_Input(const FInputActionValue &);
	void Attack_Input(const FInputActionValue &);
	void Look_Input(const FInputActionValue &);
	void Debug_SelfDamage_Input(const FInputActionValue &);
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> m_mappingCtx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_attackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_cameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_debugAction;

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
	EnhanceSystem* m_enhanceSystem;

	int32 m_hp;

	EDir_Test m_camDir;
	EDir_Test m_actorFaceDir;

	uint8 m_bCanMove : 1;
	uint8 m_bCanAttack : 1;

#pragma endregion Private Properties
	// end of Private Properties
};