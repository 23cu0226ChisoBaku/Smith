// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SmithBattleSubsystem.h"
#include "TurnControlComponent.h"

#include "AttackCommand.h"
#include "MoveCommand.h"


// Sets default values
ASmithPlayerActor::ASmithPlayerActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check((m_springArm != nullptr))

	m_springArm->SetupAttachment(RootComponent);

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check((m_cam != nullptr))

	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
	
	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->SetUsingAbsoluteRotation(true);
	m_springArm->SetWorldRotation(FRotator::ZeroRotator);
	m_springArm->TargetArmLength = 1500.0f;

	m_turnComponent = CreateDefaultSubobject<UTurnControlComponent>(TEXT("TurnComponent"));
	check((m_turnComponent != nullptr));

	m_turnComponent->SetTurnPriority(ETurnPriority::PlayerSelf);
}

// Called when the game starts or when spawned
void ASmithPlayerActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerCtrl = Cast<APlayerController>(Controller);

	check((playerCtrl != nullptr));

	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCtrl->GetLocalPlayer());

	check((enhancedInputSubsystem != nullptr));

	enhancedInputSubsystem->AddMappingContext(m_mappingCtx, 0);
	
}

void ASmithPlayerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASmithPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		const auto rotator = playerController->PlayerCameraManager->GetCameraRotation();

		UE_LOG(LogTemp, Warning, TEXT("Roll: %lf, Pitch: %lf, Yaw: %lf"), rotator.Roll, rotator.Pitch, rotator.Yaw);
	}

	USmithBattleSubsystem* sub = GetWorld()->GetSubsystem<USmithBattleSubsystem>();

	// if (sub != nullptr)
	// {
	// 	sub->YAYA();
	// }

}

// Called to bind functionality to input
void ASmithPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	inputComp->BindAction(m_moveAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Move);
	inputComp->BindAction(m_attackAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Attack);
	inputComp->BindAction(m_cameraAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Look);

}

UTurnControlComponent* ASmithPlayerActor::GetTurnControl() const
{
	check(m_turnComponent != nullptr);

  return m_turnComponent;
}

FDelegateHandle ASmithPlayerActor::Subscribe(FRequestCommandEvent::FDelegate& delegate)
{
	if (delegate.IsBound())
	{
		return m_event.Add(delegate);
	}

	return delegate.GetHandle();
}

bool ASmithPlayerActor::Unsubscribe(UObject* obj, FDelegateHandle delegateHandle)
{
	if (obj != nullptr && m_event.IsBoundToObject(obj))
	{
		m_event.Remove(delegateHandle);
		return true;
	}
	else
	{
		return false;
	}
}

void ASmithPlayerActor::Move(const FInputActionValue& value)
{
	FVector2D movementInput = value.Get<FVector2D>();

	sendCommand(MakeShared<UE::Smith::Command::MoveCommand>(nullptr));
}

void ASmithPlayerActor::Attack(const FInputActionValue& value)
{
	UE::Smith::Command::AttackCommand atkCmd(nullptr);

	sendCommand(MakeShared<UE::Smith::Command::AttackCommand>(nullptr));
}

void ASmithPlayerActor::Look(const FInputActionValue& value)
{

}

void ASmithPlayerActor::sendCommand(TSharedPtr<IBattleCommand> command)
{
	if (command == nullptr)
	{
		return;
	}

	auto turnComp = GetTurnControl();
	if (turnComp == nullptr || !turnComp->IsCommandSendable())
	{
		return;
	}

	if (!m_event.IsBound())
	{
		return;
	}

	m_event.Broadcast(this, command);

}

