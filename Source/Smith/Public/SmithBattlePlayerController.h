// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SmithBattlePlayerController.generated.h"

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASmithPlayerActor;
/**
 * 
 */
UCLASS()
class SMITH_API ASmithBattlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASmithBattlePlayerController(const FObjectInitializer&);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// Enhanced Input
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputMappingCtx)
	TObjectPtr<UInputMappingContext> MappingContext_Battle;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputMappingCtx)
	TObjectPtr<UInputMappingContext> MappingContext_Menu;

	/** 移動インプットアクション */
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> MoveAction;
	/** 攻撃インプットアクション */
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> AttackAction;
	/** カメラ移動インプットアクション */
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> CameraAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> ChangeForwardAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> UseRecoveryAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> OpenMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> CloseMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> SelectMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = PlayerInputAction)
	TObjectPtr<UInputAction> InteractMenuAction;
	/** デバッグ専用！！ */
	UPROPERTY(EditDefaultsOnly, Category = DebugInputAction)
	TObjectPtr<UInputAction> DebugAction;

private:
	// Input bind Functions
	void Move(const FInputActionValue&);
	void Attack(const FInputActionValue&);
	void ChangeCameraAngle(const FInputActionValue&);
	void ChangeForward(const FInputActionValue&);
	void Debug_SelfDamage(const FInputActionValue&);
	void OpenMenu(const FInputActionValue&);
	void CloseMenu(const FInputActionValue&);
	void UseRecovery(const FInputActionValue&);
	void InMenuSelect(const FInputActionValue&);
	void InMenuInteract(const FInputActionValue&);

public:
	UFUNCTION()
	void DisablePlayerInput();
	UFUNCTION()
	void EnablePlayerInput();

private:
	UPROPERTY()
	TObjectPtr<ASmithPlayerActor> m_player;
};
