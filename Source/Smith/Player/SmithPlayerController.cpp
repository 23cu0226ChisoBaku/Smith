// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SmithPlayerActor.h"
#include "SmithPlayerView.h"
#include "Direction.h"

#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithPlayerController)


ASmithPlayerController::ASmithPlayerController(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  // The player controller needs the tick to process inputs
  PrimaryActorTick.bCanEverTick = true;
}

void ASmithPlayerController::DisablePlayerInput()
{
  DisableInput(this);
}
void ASmithPlayerController::EnablePlayerInput()
{
  EnableInput(this);
}

void ASmithPlayerController::BeginPlay()
{
  Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check((enhancedInputSubsystem != nullptr));

  enhancedInputSubsystem->AddMappingContext(MappingContext_Battle, 0);

  m_playerModel = Cast<ASmithPlayerActor>(GetPawn());

  if (PlayerViewClass != nullptr)
  {
    m_playerGUIView = NewObject<USmithPlayerView>(this, PlayerViewClass);
    m_playerGUIView->InitPlayerView(this);
    BindModelEvents();
  }

  if (m_playerModel != nullptr)
  {
    m_playerModel->SyncronizePlayerView();
  }

}

void ASmithPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (inputComp != nullptr)
	{
		inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		inputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
		inputComp->BindAction(ChangeForwardAction, ETriggerEvent::Triggered, this, &ThisClass::ChangeForward);
		inputComp->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ThisClass::OpenMenu);
		inputComp->BindAction(CloseMenuAction, ETriggerEvent::Started, this, &ThisClass::CloseMenu);
    inputComp->BindAction(UseRecoveryAction, ETriggerEvent::Started, this, &ThisClass::UseRecovery);
		inputComp->BindAction(SelectMenuAction, ETriggerEvent::Started, this, &ThisClass::InMenuSelect);
		inputComp->BindAction(InteractMenuAction, ETriggerEvent::Started, this, &ThisClass::InMenuInteract);
    #if WITH_EDITOR
		  inputComp->BindAction(DebugAction, ETriggerEvent::Started, this, &ThisClass::Debug_SelfDamage);
    #endif

    inputComp->BindAction(CameraAction, ETriggerEvent::Started, this, &ThisClass::EnterOnlyDiagonalMoveState);
    inputComp->BindAction(CameraAction, ETriggerEvent::Completed, this, &ThisClass::ExitOnlyDiagonalMoveState);
	}

}

void ASmithPlayerController::Move(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  const FVector2D movementInput = inputValue.Get<FVector2D>();
  if (movementInput.IsNearlyZero())
  {
    return;
  }
	
  m_playerModel->Move(movementInput);
}
void ASmithPlayerController::Attack(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->Attack();
}

void ASmithPlayerController::ChangeForward(const FInputActionValue& inputValue)
{  
  if (m_playerModel == nullptr)
  {
    return;
  }

  const FVector2D changeForwardInput = inputValue.Get<FVector2D>();

  m_playerModel->ChangeForward(changeForwardInput);
}

#if WITH_EDITOR
void ASmithPlayerController::Debug_SelfDamage(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->SelfDamage_Debug(1000);
}
#endif 

void ASmithPlayerController::OpenMenu(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->OpenMenu();
}

void ASmithPlayerController::CloseMenu(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->CloseMenu();
}

void ASmithPlayerController::UseRecovery(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->RecoverHealth();
}
void ASmithPlayerController::InMenuSelect(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  float selectDirection = inputValue.Get<float>();
  m_playerModel->SelectNextMenuItem(selectDirection);
}
void ASmithPlayerController::InMenuInteract(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->InteractMenu();
}

void ASmithPlayerController::EnterOnlyDiagonalMoveState(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->SetOnlyReceiveDiagonalInput(true);
}
void ASmithPlayerController::ExitOnlyDiagonalMoveState(const FInputActionValue& inputValue)
{
  if (m_playerModel == nullptr)
  {
    return;
  }

  m_playerModel->SetOnlyReceiveDiagonalInput(false);
}

void ASmithPlayerController::BindModelEvents()
{
  check(m_playerModel != nullptr);
  check(m_playerGUIView != nullptr);

  m_playerModel->OnHerbValueChanged.AddUObject(m_playerGUIView, &USmithPlayerView::SetHerbValue);
  m_playerModel->OnHealthChanged.AddUObject(m_playerGUIView, &USmithPlayerView::SetHP);

  m_playerModel->OnEnhanceMenuOpened.AddUObject(m_playerGUIView, &USmithPlayerView::ShowUpgradeMenu);
  m_playerModel->OnEnhanceMenuOpened.AddUObject(this, &ThisClass::SwitchIMC_Menu);
  m_playerModel->OnEnhanceMenuOpened.AddLambda(
                                                [this]()
                                                {
                                                  if (OnEnhanceMenuOpened.IsBound())
                                                  {
                                                    OnEnhanceMenuOpened.Broadcast();
                                                  }
                                                }
                                              );

  m_playerModel->OnEnhanceMenuClosed.AddUObject(m_playerGUIView, &USmithPlayerView::HideUpgradeMenu);
  m_playerModel->OnEnhanceMenuClosed.AddUObject(this, &ThisClass::SwitchIMC_Battle);
  m_playerModel->OnEnhanceMenuClosed.AddLambda(
                                                [this]()
                                                {
                                                  if (OnEnhanceMenuClosed.IsBound())
                                                  {
                                                    OnEnhanceMenuClosed.Broadcast();
                                                  }
                                                }
                                              );

  m_playerModel->OnEnhanceMenuInitialized.BindUObject(m_playerGUIView, &USmithPlayerView::InitEnhanceMenuInfo);
  m_playerModel->OnMenuItemChangeFocus.AddUObject(m_playerGUIView, &USmithPlayerView::SelectNextMenuItem);
  m_playerModel->OnItemSelected.BindUObject(m_playerGUIView, &USmithPlayerView::GetSelectingItemIdx);
  
}

void ASmithPlayerController::SwitchIMC_Battle()
{
  UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (enhancedInputSubsystem != nullptr)
  {
    enhancedInputSubsystem->RemoveMappingContext(MappingContext_Menu);
    enhancedInputSubsystem->AddMappingContext(MappingContext_Battle, 0);
  }
}

void ASmithPlayerController::SwitchIMC_Menu()
{
    UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (enhancedInputSubsystem != nullptr)
  {
    enhancedInputSubsystem->RemoveMappingContext(MappingContext_Battle);
    enhancedInputSubsystem->AddMappingContext(MappingContext_Menu, 0);
  }
}
