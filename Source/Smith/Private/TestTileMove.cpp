// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTileMove.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

namespace Smith_NS_Mapinfo
{
	constexpr float TILE_SIZE = 250.0f;
}

// Sets default values
ATestTileMove::ATestTileMove()
	: m_capsuleCol(nullptr)
	, m_springArm(nullptr)
	, m_cam(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_capsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("testCapsule"));

	check((m_capsuleCol != nullptr))

	m_capsuleCol->SetupAttachment(RootComponent);
	m_capsuleCol->InitCapsuleSize(90.0f, 240.0f);

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("testSpringArm"));

	check((m_springArm != nullptr))

	m_springArm->SetupAttachment(m_capsuleCol);

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("testCam"));

	check((m_cam != nullptr))

	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
	
	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->SetRelativeRotation(FRotator { 0.0f , 0.0f , 0.0f });
	m_springArm->TargetArmLength = 500.0f;

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

	moveTile_test();
	attack_test();
	updateCam_test();

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

void ATestTileMove::updateCam_test()
{
	if (::IsValid(m_cam))
	{

	}
}

void ATestTileMove::SetCameraPos(const FVector& pos)
{
	if (!::IsValid(m_cam))
	{
		return;
	}

	m_cam->SetRelativeLocation(pos);
}

void ATestTileMove::SetCameraAngle(const FRotator& rotate)
{
	if (!::IsValid(m_cam))
	{
		return;
	}

	m_cam->SetRelativeRotation(rotate);
}

