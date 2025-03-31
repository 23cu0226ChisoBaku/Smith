// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SmithPlayerActor.h"
#include "SmithPlayerView.h"
#include "SmithModelHelperFunctionLibrary.h"
#include "Direction.h"

#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithPlayerController)

namespace SmithPlayerController::Private
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

    // TODO ゼロ補正
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

}

void ASmithPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(InputComponent);

	if (::IsValid(inputComp))
	{
		inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		inputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
		inputComp->BindAction(ChangeForwardAction, ETriggerEvent::Triggered, this, &ThisClass::ChangeForward);
		inputComp->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ThisClass::OpenMenu);
		inputComp->BindAction(CloseMenuAction, ETriggerEvent::Started, this, &ThisClass::CloseMenu);
    inputComp->BindAction(UseRecoveryAction, ETriggerEvent::Started, this, &ThisClass::UseRecovery);
		inputComp->BindAction(SelectMenuAction, ETriggerEvent::Started, this, &ThisClass::InMenuSelect);
		inputComp->BindAction(InteractMenuAction, ETriggerEvent::Started, this, &ThisClass::InMenuInteract);
		inputComp->BindAction(DebugAction, ETriggerEvent::Started, this, &ThisClass::Debug_SelfDamage);

    // Debug
    inputComp->BindAction(CameraAction, ETriggerEvent::Started, this, &ThisClass::Debug_SetOnlyDiagonalMoveState);
    inputComp->BindAction(CameraAction, ETriggerEvent::Completed, this, &ThisClass::Debug_RemoveOnlyDiagonalMoveState);
	}

}

void ASmithPlayerController::Move(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }
  
  using namespace SmithPlayerController::Private;
  const FVector2D movementInput = inputValue.Get<FVector2D>();
  if (movementInput.SquaredLength() < MOVE_DEAD_ZONE_SCALAR)
  {
    return;
  }

  const EDirection cameraDirection = m_playerModel->GetCameraDirection();
  const EDirection newDirection = CalculateDirectionRelativeCamera(cameraDirection, movementInput, m_bIsDiagonal);
	
  m_playerModel->Move(newDirection);
}
void ASmithPlayerController::Attack(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->Attack();
}

void ASmithPlayerController::ChangeForward(const FInputActionValue& inputValue)
{  
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  using namespace SmithPlayerController::Private;
  const FVector2D changeForwardInput = inputValue.Get<FVector2D>();
  const EDirection cameraDirection = m_playerModel->GetCameraDirection();
  const EDirection newDirection = CalculateDirectionRelativeCamera(cameraDirection, changeForwardInput);

  m_playerModel->ChangeForward(newDirection);
}

#if WITH_EDITOR
void ASmithPlayerController::Debug_SelfDamage(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->SelfDamage_Debug(1000);
}
#endif 

void ASmithPlayerController::OpenMenu(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->OpenMenu();

}

void ASmithPlayerController::CloseMenu(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->CloseMenu();
}

void ASmithPlayerController::UseRecovery(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->RecoverHealth();
}
void ASmithPlayerController::InMenuSelect(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  float selectDirection = inputValue.Get<float>();
  m_playerModel->SelectNextMenuItem(selectDirection);
}
void ASmithPlayerController::InMenuInteract(const FInputActionValue& inputValue)
{
  if ((m_playerModel == nullptr) || (!m_playerModel->CanReceiveInputEvent()))
  {
    return;
  }

  m_playerModel->InteractMenu();
}

void ASmithPlayerController::Debug_SetOnlyDiagonalMoveState(const FInputActionValue& inputValue)
{
  m_bIsDiagonal = true;
}
void ASmithPlayerController::Debug_RemoveOnlyDiagonalMoveState(const FInputActionValue& inputValue)
{
  m_bIsDiagonal = false;
}

void ASmithPlayerController::BindModelEvents()
{
  check(m_playerModel != nullptr);
  check(m_playerGUIView != nullptr);

  m_playerModel->OnHerbValueChanged.AddUObject(m_playerGUIView, &USmithPlayerView::SetHerbValue);
  m_playerModel->OnHealthChanged.AddUObject(m_playerGUIView, &USmithPlayerView::SetHP);

  m_playerModel->OnEnhanceMenuOpened.AddUObject(m_playerGUIView, &USmithPlayerView::ShowUpgradeMenu);
  m_playerModel->OnEnhanceMenuOpened.AddUObject(this, &ThisClass::SwitchIMC_Menu);
  m_playerModel->OnEnhanceMenuOpened.AddUObject(this, &ThisClass::OnOpenMenuDelegate);
  m_playerModel->OnEnhanceMenuClosed.AddUObject(m_playerGUIView, &USmithPlayerView::HideUpgradeMenu);
  m_playerModel->OnEnhanceMenuClosed.AddUObject(this, &ThisClass::SwitchIMC_Battle);
  m_playerModel->OnEnhanceMenuClosed.AddUObject(this, &ThisClass::OnCloseMenuDelegate);

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

void ASmithPlayerController::OnOpenMenuDelegate()
{
  if (OnEnhanceMenuOpened.IsBound())
  {
    OnEnhanceMenuOpened.Broadcast();
  }
}

void ASmithPlayerController::OnCloseMenuDelegate()
{
  if (OnEnhanceMenuClosed.IsBound())
  {
    OnEnhanceMenuClosed.Broadcast();
  }
}