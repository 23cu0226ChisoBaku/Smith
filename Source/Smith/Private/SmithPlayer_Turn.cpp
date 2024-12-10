// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayer_Turn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ASmithPlayer_Turn::ASmithPlayer_Turn()
{
  PrimaryActorTick.bCanEverTick = true;

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("testCameraBoom"));

	check((m_springArm != nullptr))

	m_springArm->SetupAttachment(RootComponent);

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("testCam"));

	check((m_cam != nullptr))

	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
	
	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->SetUsingAbsoluteRotation(true);
	m_springArm->SetWorldRotation(FRotator::ZeroRotator);
	m_springArm->TargetArmLength = 1500.0f;


}

void ASmithPlayer_Turn::BeginPlay()
{
  Super::BeginPlay();

  APlayerController* playerCtrl = GetWorld()->GetFirstPlayerController();
  if (playerCtrl != nullptr)
  {
    EnableInput(playerCtrl);
    playerCtrl->PlayerCameraManager->SetViewTarget(this);
  }

  if (::IsValid(InputComponent))
  {
    UEnhancedInputComponent* playerInput = Cast<UEnhancedInputComponent>(InputComponent);

    if (::IsValid(playerInput))
    {
      //playerInput->BindAction();
    }
  }  
  else
  {
    if (GEngine != nullptr)
    {
      GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Cant enable player input"));
    }
  }
}

void ASmithPlayer_Turn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void ASmithPlayer_Turn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

