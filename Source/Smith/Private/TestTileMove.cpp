// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTileMove.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATestTileMove::ATestTileMove()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("testSpringArm"));

	check((m_springArm != nullptr))

	m_springArm->SetupAttachment(RootComponent);

	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->TargetArmLength = 500.0f;

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("testCam"));

	check((m_cam != nullptr))

	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("aaaa"));
	}

}

// Called when the game starts or when spawned
void ATestTileMove::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTileMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("aaaa"));
	}

}

// Called to bind functionality to input
void ATestTileMove::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ATestTileMove::moveTile_test()
{
	return false;
}

bool ATestTileMove::attack_test()
{
	return false;
}

