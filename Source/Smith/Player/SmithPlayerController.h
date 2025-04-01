// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"

#include "SmithPlayerController.generated.h"

// Unreal Enhanced Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class ASmithPlayerActor;
class USmithPlayerView;

UCLASS()
class SMITH_API ASmithPlayerController : public APlayerController
{
  GENERATED_BODY()

public:

  TMulticastDelegate<void()> OnEnhanceMenuOpened;
  TMulticastDelegate<void()> OnEnhanceMenuClosed;

public:

  ASmithPlayerController(const FObjectInitializer&);

  UFUNCTION()
  void DisablePlayerInput();

  UFUNCTION()
  void EnablePlayerInput();
  

private:

  UPROPERTY(EditDefaultsOnly, Category = "Input|IMC")
  TObjectPtr<UInputMappingContext> MappingContext_Battle;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IMC")
  TObjectPtr<UInputMappingContext> MappingContext_Menu;

  /** 移動インプットアクション */
  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> MoveAction;

  /** 攻撃インプットアクション */
  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> AttackAction;

  /** カメラ移動インプットアクション */
  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> CameraAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> ChangeForwardAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> UseRecoveryAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> OpenMenuAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> CloseMenuAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> SelectMenuAction;

  UPROPERTY(EditDefaultsOnly, Category = "Input|IA")
  TObjectPtr<UInputAction> InteractMenuAction;

  #if WITH_EDITORONLY_DATA
  /** デバッグ専用！！ */
  UPROPERTY(EditDefaultsOnly, Category = "Input|Debug|IA")
  TObjectPtr<UInputAction> DebugAction;
  #endif

  UPROPERTY(EditDefaultsOnly, Category = "PlayerView")
  TSubclassOf<USmithPlayerView> PlayerViewClass;

protected:

  //---Begin of AActor Interface
  virtual void BeginPlay() override;
  //---End of AActor Interface

  //---Begin of APlayerController Interface
  virtual void SetupInputComponent() override;
  //---End of APlayerController Interface

private:
  // Input bind Functions
  void Move(const FInputActionValue&);
  void Attack(const FInputActionValue&);
  void ChangeForward(const FInputActionValue&);
  void OpenMenu(const FInputActionValue&);
  void CloseMenu(const FInputActionValue&);
  void UseRecovery(const FInputActionValue&);
  void InMenuSelect(const FInputActionValue&);
  void InMenuInteract(const FInputActionValue&);
  void EnterOnlyDiagonalMoveState(const FInputActionValue&);
  void ExitOnlyDiagonalMoveState(const FInputActionValue&);

  #if WITH_EDITOR
  void Debug_SelfDamage(const FInputActionValue&);
  #endif

  void BindModelEvents();
  void SwitchIMC_Battle();
  void SwitchIMC_Menu();

private:

  UPROPERTY()
  TObjectPtr<ASmithPlayerActor> m_playerModel;

  UPROPERTY()
  TObjectPtr<USmithPlayerView> m_playerGUIView;

};
