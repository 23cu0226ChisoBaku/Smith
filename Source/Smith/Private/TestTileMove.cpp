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
	, m_nextDir(FVector::ZeroVector)
	, m_camDir(North)
	, m_hasMoveInput(false)
	, m_hasAttackInput(false)
	, m_hasLookInput(false)
	, m_isInAction(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_capsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("testCapsule"));

	check((m_capsuleCol != nullptr))

	m_capsuleCol->SetupAttachment(RootComponent);
	m_capsuleCol->InitCapsuleSize(90.0f, 240.0f);

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("testCameraBoom"));

	check((m_springArm != nullptr))

	m_springArm->SetupAttachment(m_capsuleCol);

	m_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("testCam"));

	check((m_cam != nullptr))

	m_cam->SetupAttachment(m_springArm, USpringArmComponent::SocketName);
	
	m_springArm->bDoCollisionTest = false;
	m_springArm->bEnableCameraLag = false;
	m_springArm->bEnableCameraRotationLag = false;
	m_springArm->SetUsingAbsoluteRotation(true);
	m_springArm->SetWorldRotation(FRotator::ZeroRotator);
	m_springArm->TargetArmLength = 1500.0f;

	m_camDir = North;

}

// Called when the game starts or when spawned
void ATestTileMove::BeginPlay()
{
	Super::BeginPlay();

	setupInputMappingCtx_test();
	
	const FVector playerForward = GetActorForwardVector();

	FString forwardStr;
	forwardStr.AppendChars(TEXT("X:"),3 );
	forwardStr.Append(FString::SanitizeFloat(playerForward.X));
	forwardStr.AppendChars(TEXT("Y:"),3 );
	forwardStr.Append(FString::SanitizeFloat(playerForward.Y));
	forwardStr.AppendChars(TEXT("Z:"),3 );
	forwardStr.Append(FString::SanitizeFloat(playerForward.Z));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, forwardStr);
	}
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

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(!::IsValid(inputComp))
	{
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("mdiopjfljdjlsjlksdf"));
		}
		return;

	}

	inputComp->BindAction(m_moveAction, ETriggerEvent::Started, this, &ATestTileMove::Move);
	inputComp->BindAction(m_attackAction, ETriggerEvent::Started, this, &ATestTileMove::Attack);
	inputComp->BindAction(m_cameraAction, ETriggerEvent::Started, this, &ATestTileMove::Look);

}

bool ATestTileMove::moveTile_test()
{
	if (!m_hasMoveInput)
	{
		return false;
	}

	SetActorLocation(m_nextDir);
	m_nextDir = FVector::ZeroVector;
	m_hasMoveInput = false;
	return true;
}

bool ATestTileMove::attack_test()
{
	const FRotator playerRotator = GetActorRotation();

	return false;
}

bool ATestTileMove::updateCam_test()
{
	if (!::IsValid(m_springArm))
	{
		return false;
	}

	if (!m_hasLookInput)
	{
		return false;
	}

	double springArmRotateY = StaticCast<double>(m_camDir) * 45.0;
	m_springArm->SetWorldRotation(FRotator(0.0, springArmRotateY, 0.0));
	m_hasLookInput = false;

	return true;

}

bool ATestTileMove::changeForward_test(const FVector2D& inputValue)
{
	const double cameraAngle = FMath::DegreesToRadians(StaticCast<double>(m_camDir * 45));
	double directionX = inputValue.Y * cos(cameraAngle) - inputValue.X * sin(cameraAngle);
	double directionY = inputValue.Y * sin(cameraAngle) + inputValue.X * cos(cameraAngle);

	if (FMath::IsNearlyZero(directionX))
	{
		directionX = 0.0;
	}

	if (FMath::IsNearlyZero(directionY))
	{
		directionY = 0.0;
	}

	const FVector DirectionVec = FVector(directionX, directionY, 0.0);
	EDir_Test newDir = vectorToEDir(DirectionVec);

	SetActorRelativeRotation(FRotator(0.0, newDir * 45.0, 0.0));

	return true;
}

void ATestTileMove::setupInputMappingCtx_test()
{
	APlayerController* playerCtrl = Cast<APlayerController>(Controller);

	if (!::IsValid(playerCtrl))
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("adnasidjasod"));
		}
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCtrl->GetLocalPlayer());
	if (!::IsValid(subsys))
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("adnasidjasod"));
		}
		return;
	}

	subsys->AddMappingContext(m_mappingCtx, 0);
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

void ATestTileMove::Move(const FInputActionValue& value)
{
	FVector2D movementInput = value.Get<FVector2D>();
	changeForward_test(movementInput);

	if (m_hasMoveInput || m_isInAction)
	{
		return;
	}

	m_hasMoveInput = true;
	const FVector startPos = GetActorLocation();
	const double cameraAngle = FMath::DegreesToRadians(StaticCast<double>(m_camDir * 45));
	double directionX = movementInput.Y * cos(cameraAngle) - movementInput.X * sin(cameraAngle);
	double directionY = movementInput.Y * sin(cameraAngle) + movementInput.X * cos(cameraAngle);

	if (FMath::IsNearlyZero(directionX))
	{
		directionX = 0.0;
	}

	if (FMath::IsNearlyZero(directionY))
	{
		directionY = 0.0;
	}
	
	const FVector direction = FVector(directionX, directionY, 0.0);

	m_nextDir = startPos + direction * Smith_NS_Mapinfo::TILE_SIZE;

}

void ATestTileMove::Attack(const FInputActionValue& value)
{
	if (m_hasAttackInput || m_isInAction)
	{
		return;
	}

	m_hasAttackInput = true;
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Attack"));
	}
}

void ATestTileMove::Look(const FInputActionValue& value)
{
	if (m_hasLookInput || m_isInAction)
	{
		return;
	}

	const FVector2D lookInput = value.Get<FVector2D>();
	m_hasLookInput = true;
	uint8 newDir = m_camDir;

	if (lookInput.X > 0.0f)
	{
		newDir += East;
	}
	else if (lookInput.X < 0.0f)
	{
		newDir += West;
	}

	m_camDir = StaticCast<EDir_Test>(newDir % DirectionCnt);

}

ATestTileMove::EDir_Test ATestTileMove::vectorToEDir(const FVector& direction)
{
	const double dot = direction.Dot(FVector::ForwardVector);
	const FVector dir = direction.Cross(FVector::ForwardVector);
	const double newAngle = FMath::RadiansToDegrees(acos(dot)) + (dir.Z > 0.0 ? 180.0 : 0.0); 

	const double anglePerDirection = 360.0 / StaticCast<double>(DirectionCnt);
	return StaticCast<EDir_Test>(round(newAngle / anglePerDirection));
}



