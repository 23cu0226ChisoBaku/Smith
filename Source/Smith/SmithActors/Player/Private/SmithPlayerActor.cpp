// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SmithBattleSubsystem.h"
#include "TurnControlComponent.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"

#include "AttackCommand.h"
#include "MoveCommand.h"

#include "AttackHandle.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

namespace SmithPlayerActor::Private
{
	constexpr int32 Temp_Player_HP = 30;
	constexpr float TILE_SIZE = 250.0f;

	// Temp Capsule Size
	// TODO obsolete when map is done
	constexpr float CAPSULE_RADIUS = 90.0f;
	constexpr float CAPSULE_HALF_HEIGHT = 240.0f;

	ASmithPlayerActor::EDir_Test VectorToEDir(const FVector& direction)
	{
		const double dot = direction.Dot(FVector::ForwardVector);
		const FVector dir = direction.Cross(FVector::ForwardVector);
		const double newAngle = FMath::RadiansToDegrees(acos(dot)) + (dir.Z > 0.0 ? 180.0 : 0.0); 

		const double anglePerDirection = 360.0 / StaticCast<double>(ASmithPlayerActor::DirectionCnt);
		return StaticCast<ASmithPlayerActor::EDir_Test>(round(newAngle / anglePerDirection));
	}
}

// Sets default values
ASmithPlayerActor::ASmithPlayerActor()
	: m_event({})
	, m_hp(0)
	, m_camDir(North)
	, m_actorFaceDir(North)
	, m_bCanMove(true)
	, m_bCanAttack(true)
{
	using namespace SmithPlayerActor::Private;
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
	m_springArm->TargetArmLength = 1500.0f * FMath::Pow(2.0, 0.5);

	const FVector actorFwd = GetActorLocation().GetUnsafeNormal();

	if (actorFwd != FVector::ZeroVector)
	{
		m_camDir = VectorToEDir(actorFwd);
	}

	m_turnComponent = CreateDefaultSubobject<UTurnControlComponent>(TEXT("TurnComponent"));
	check((m_turnComponent != nullptr));

	m_turnComponent->SetTurnPriority(ETurnPriority::PlayerSelf);

	m_moveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("Smith MoveComponent"));
	check((m_moveComponent != nullptr));

	m_moveComponent->SetMoveSpeed(TILE_SIZE);

	m_hp = Temp_Player_HP;

	m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("Smith AttackComponent"));

	check((m_atkComponent != nullptr));

}

// Called when the game starts or when spawned
void ASmithPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (::IsValid(m_springArm))
	{
		m_springArm->SetWorldRotation(FRotator{315.0, 0.0, 0.0});
	}

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

	if (m_hp <= 0)
	{
		return;
	}
	
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		const auto rotator = playerController->PlayerCameraManager->GetCameraRotation();

		UE_LOG(LogTemp, Warning, TEXT("Roll: %lf, Pitch: %lf, Yaw: %lf"), rotator.Roll, rotator.Pitch, rotator.Yaw);
	}

}

// Called to bind functionality to input
void ASmithPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	inputComp->BindAction(m_moveAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Move_Input);
	inputComp->BindAction(m_attackAction, ETriggerEvent::Started, this, &ASmithPlayerActor::Attack_Input);
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

void ASmithPlayerActor::Move_Input(const FInputActionValue& value)
{
	using namespace SmithPlayerActor::Private;

	if (!::IsValid(m_turnComponent) || !m_turnComponent->IsCommandSendable())
	{
		return;
	}

	FVector2D movementInput = value.Get<FVector2D>();

	if (movementInput == FVector2D::ZeroVector)
	{
		return;
	}

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

	const EDir_Test newDir = VectorToEDir(FVector{directionX, directionY, 0.0});

	changeFwdImpl(newDir);

	moveImpl(FVector2D{directionX, directionY});
}

void ASmithPlayerActor::Attack_Input(const FInputActionValue& value)
{
	if (!::IsValid(m_turnComponent) || !m_turnComponent->IsCommandSendable())
	{
		return;
	}

	attackImpl();
}

// TODO Test
void ASmithPlayerActor::Look(const FInputActionValue& value)
{
	OnAttack(
						{ 
							TEXT("God"),		// Attack
						  10,							// Damage
						}
					);
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

void ASmithPlayerActor::moveImpl(FVector2D moveDir)
{
	using namespace SmithPlayerActor::Private;

	FHitResult hit = {};

	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + FVector{moveDir.X, moveDir.Y, 0.0} * TILE_SIZE;

	FCollisionShape sphereShape {};
	FCollisionQueryParams hitParam;
	hitParam.AddIgnoredActor(this);
	const float sphereRadius = CAPSULE_HALF_HEIGHT * 0.5f;
	sphereShape.SetSphere(sphereRadius);
	
	const bool isHit = GetWorld()->SweepSingleByChannel(
																											hit, 
																											GetActorLocation(),
																											endPos,
																											FQuat::Identity,
																											ECollisionChannel::ECC_MAX,
																											sphereShape,
																											hitParam
																										 );

	if (!isHit && ::IsValid(m_moveComponent))
	{
		m_moveComponent->SetTerminusPos(endPos);
		sendCommand(MakeShared<UE::Smith::Command::MoveCommand>(m_moveComponent));
	}
}

void ASmithPlayerActor::attackImpl()
{
	TArray<AActor*> hitActors {};

	const FVector atkDir = GetActorForwardVector();

	if (searchActorsInDirection(atkDir, hitActors))
	{
		for(auto actorPtr : hitActors)
		{
			IAttackable* attackable = Cast<IAttackable>(actorPtr);
			sendCommand(MakeShared<UE::Smith::Command::AttackCommand>(m_atkComponent, attackable, AttackHandle{GetName(), 3}));
		}
	}
	else
	{
		sendCommand(MakeShared<UE::Smith::Command::AttackCommand>(nullptr, nullptr, AttackHandle{}));
	}
}

void ASmithPlayerActor::changeFwdImpl(EDir_Test newDirection)
{
	if (m_actorFaceDir == newDirection)
	{
		return;
	}

	m_actorFaceDir = newDirection;

	const double newYaw = StaticCast<double>(m_actorFaceDir) * 45.0;
	SetActorRelativeRotation(FRotator(0.0, newYaw, 0.0));
}

bool ASmithPlayerActor::searchActorsInDirection(FVector direction, TArray<AActor*>& OutResult)
{
	using namespace SmithPlayerActor::Private;

	OutResult.Empty();

	TArray<FHitResult> hits = {};

	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + direction * TILE_SIZE;

	FCollisionQueryParams hitParam;
	hitParam.AddIgnoredActor(this);
	const float sphereRadius = CAPSULE_HALF_HEIGHT * 0.5f;
	
	const bool isHit = GetWorld()->SweepMultiByChannel(
																											hits, 
																											GetActorLocation(),
																											endPos,
																											FQuat::Identity,
																											ECollisionChannel::ECC_MAX,
																											FCollisionShape::MakeSphere(sphereRadius),
																											hitParam
																										);

	if (isHit)
	{
		for(const auto& hitResult: hits)
		{
			OutResult.Emplace(hitResult.GetActor());
		}
	}

	return isHit;
}

void ASmithPlayerActor::OnAttack(AttackHandle&& attack)
{
	m_hp -= attack.AttackPower;

	FString msg{};
	msg.Append(GetName());
	msg.Append(TEXT(" left HP:"));
	msg.Append(FString::FromInt(m_hp));
	MDebug::LogError(msg);

	if (m_hp <= 0)
	{
		MDebug::LogError(TEXT("Player is dead"));

		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*(GetWorld()->GetName())), false);
		DisableInput(Cast<APlayerController>(Controller));
	}
}