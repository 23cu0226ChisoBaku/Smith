// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattlePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SmithPlayerActor.h"
#include "Direction.h"

#include "MLibrary.h"

namespace SmithPlayerController::Private
{
  constexpr double ANGLE_PER_DIRECTION = 360.0 / (double)EDirection::DirectionCount;
  constexpr double MOVE_DEAD_ZONE_SCALAR = 0.5;
  constexpr uint8 DIRECTION_COUNT = (uint8)EDirection::DirectionCount;
  // ベクトルを方向列挙に変換する(X,Yだけ,Zは無視)
	EDirection VectorDirToEDir(const FVector& direction)
	{
		const double dot = direction.Dot(FVector::ForwardVector);
		const FVector cross = direction.Cross(FVector::ForwardVector);

		const double angle = FMath::RadiansToDegrees(acos(dot));
		// (1,0,0)(※EDirection::Northを表すベクトル)から時計回りに回転しdirectionまで回転した角度を計算
		const double angleClockwise =  cross.Z > 0.0 ? (360.0 - angle) : angle; 
		const EDirection dir = StaticCast<EDirection>(StaticCast<uint8>(round(angleClockwise / ANGLE_PER_DIRECTION)) % DIRECTION_COUNT);
		return dir;
	}

  EDirection CalculateDirectionRelativeCamera(EDirection cameraDirection, const FVector2D& inputValue)
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

    const EDirection newDirection = VectorDirToEDir(FVector{directionX, directionY, 0.0});
    return newDirection;
  }
}

ASmithBattlePlayerController::ASmithBattlePlayerController(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  // The player controller needs the tick to process inputs
  PrimaryActorTick.bCanEverTick = true;
}

void ASmithBattlePlayerController::BeginPlay()
{
  Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check((enhancedInputSubsystem != nullptr));

  enhancedInputSubsystem->AddMappingContext(MappingContext_Battle, 0);

  m_player = Cast<ASmithPlayerActor>(GetPawn());
  if (m_player != nullptr)
  {
    m_player->OnStartCameraRotation.AddUObject(this, &ASmithBattlePlayerController::DisablePlayerInput);
    m_player->OnFinishCameraRotation.AddUObject(this, &ASmithBattlePlayerController::EnablePlayerInput);
  }

}

void ASmithBattlePlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(InputComponent);

	if (::IsValid(inputComp))
	{
		inputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASmithBattlePlayerController::Move);
		inputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::Attack);
		inputComp->BindAction(CameraAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::ChangeCameraAngle);
		inputComp->BindAction(ChangeForwardAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::ChangeForward);
		inputComp->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::OpenMenu);
		inputComp->BindAction(CloseMenuAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::CloseMenu);
    inputComp->BindAction(UseRecoveryAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::UseRecovery);
		inputComp->BindAction(SelectMenuAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::InMenuSelect);
		inputComp->BindAction(InteractMenuAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::InMenuInteract);
		inputComp->BindAction(DebugAction, ETriggerEvent::Started, this, &ASmithBattlePlayerController::Debug_SelfDamage);
	}

}

void ASmithBattlePlayerController::Move(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }
  
  using namespace SmithPlayerController::Private;
  const FVector2D movementInput = inputValue.Get<FVector2D>();
  if (movementInput.SquaredLength() < MOVE_DEAD_ZONE_SCALAR)
  {
    return;
  }

  const EDirection cameraDirection = m_player->GetCameraDirection();
  const EDirection newDirection = CalculateDirectionRelativeCamera(cameraDirection, movementInput);
	
  m_player->Move(newDirection);
}
void ASmithBattlePlayerController::Attack(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  m_player->Attack();
}
void ASmithBattlePlayerController::ChangeCameraAngle(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }
  float inputDirection = inputValue.Get<float>();
  bool bIsClockwise = false;
	uint8 newDirection = StaticCast<uint8>(m_player->GetCameraDirection());

	if (inputDirection > 0.0f)
	{
		newDirection += StaticCast<uint8>(EDirection::East);
		bIsClockwise = true;
	}
	else if (inputDirection < 0.0f)
	{
		newDirection += StaticCast<uint8>(EDirection::West);
		bIsClockwise = false;
	}

  m_player->ChangeCameraDirection(StaticCast<EDirection>(newDirection % StaticCast<uint8>(EDirection::DirectionCount)), bIsClockwise);
}
void ASmithBattlePlayerController::ChangeForward(const FInputActionValue& inputValue)
{  
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  using namespace SmithPlayerController::Private;
  const FVector2D changeForwardInput = inputValue.Get<FVector2D>();
  const EDirection cameraDirection = m_player->GetCameraDirection();
  const EDirection newDirection = CalculateDirectionRelativeCamera(cameraDirection, changeForwardInput);

  m_player->ChangeForward(newDirection);
}
void ASmithBattlePlayerController::Debug_SelfDamage(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  m_player->SelfDamage_Debug(1000);
}
void ASmithBattlePlayerController::OpenMenu(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }
  UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (enhancedInputSubsystem != nullptr)
  {
    enhancedInputSubsystem->RemoveMappingContext(MappingContext_Battle);
    enhancedInputSubsystem->AddMappingContext(MappingContext_Menu, 0);
  }

  m_player->OpenMenu();
}

void ASmithBattlePlayerController::CloseMenu(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
  if (enhancedInputSubsystem != nullptr)
  {
    enhancedInputSubsystem->RemoveMappingContext(MappingContext_Menu);
    enhancedInputSubsystem->AddMappingContext(MappingContext_Battle, 0);
  }

  m_player->CloseMenu();
}

void ASmithBattlePlayerController::UseRecovery(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  m_player->RecoverHealth();
}
void ASmithBattlePlayerController::InMenuSelect(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  float selectDirection = inputValue.Get<float>();
  m_player->SelectNextMenuItem(selectDirection);
}
void ASmithBattlePlayerController::InMenuInteract(const FInputActionValue& inputValue)
{
  if (m_player == nullptr || !m_player->CanReceiveInputEvent())
  {
    return;
  }

  if (m_player->InteractMenu())
  {
    UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (enhancedInputSubsystem != nullptr)
    {
      enhancedInputSubsystem->RemoveMappingContext(MappingContext_Menu);
      enhancedInputSubsystem->AddMappingContext(MappingContext_Battle, 0);
    }
  }
}

void ASmithBattlePlayerController::DisablePlayerInput()
{
  DisableInput(this);
}
void ASmithBattlePlayerController::EnablePlayerInput()
{
  EnableInput(this);
}