#include "MyEnemy.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TurnControlComponent.h"
#include "MoveCommand.h"
#include "AttackCommand.h"
#include "IAttackable.h"
#include "SmithPlayerActor.h"

#include "Debug.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UTurnControlComponent *turnComp = CreateDefaultSubobject<UTurnControlComponent>(TEXT("Konno Enemy Turn CTRL"));

	if (turnComp != nullptr)
	{
		AddInstanceComponent(turnComp);
	}

	m_turnCtrl = turnComp;

	m_turnCtrl->SetTurnPriority(ETurnPriority::Rival);

	USmithMoveComponent *moveComp = CreateDefaultSubobject<USmithMoveComponent>(TEXT("konno Enemy Move Component"));

	if (moveComp)
	{
		AddInstanceComponent(moveComp);
	}

	m_moveComp = moveComp;
	m_moveComp->SetMoveSpeed(250.0f);
	MOVE_DISTANCE = 250.0f;
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 指定したクラスのアクターを取得
	TArray<TObjectPtr<AActor>> aActorList;
	UGameplayStatics::GetAllActorsOfClass(this, ASmithPlayerActor::StaticClass(), aActorList);

	for (TObjectPtr<AActor> aActor : aActorList)
	{
		// 取得したクラスにキャスト
		m_target = Cast<ASmithPlayerActor>(aActor);
		break;
	}
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_timer += DeltaTime;
	if (m_timer > 5.0f)
	{
		PlayerCheck();
		m_timer = 0.0f;
	}
}

void AMyEnemy::PlayerCheck()
{
	const float rayLenth = 100.0f;
	const FVector StartLocation = GetActorLocation();

	const FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth};

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	AActor *HitActor = nullptr;
	bool bHit = false;

	for (int i = 0; i < 4; ++i)
	{
		// レイキャスト
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation[i],
			ECC_PhysicsBody,
			CollisionParams);

		if (!bHit)
		{
			continue;
		}

		HitActor = HitResult.GetActor();

		// Playerにヒットしていたら攻撃
		if (::IsValid(HitActor))
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

			IAttackable* attackable = Cast<IAttackable>(HitActor);

			// TODO麦くんが直す
			m_event.Broadcast(this, MakeShared<UE::Smith::Command::AttackCommand>(nullptr));
			return;
		}
	}

	// レイのデバッグ
	for (int i = 0; i < 4; ++i)
	{
		DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);
		DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
	}

	UTurnControlComponent *turnCtrl = GetTurnControl();
	if (turnCtrl != nullptr && turnCtrl->IsCommandSendable())
	{
		// 移動の処理
		if (m_event.IsBound())
		{
			m_moveComp->SetTerminusPos(MoveDirection());
			m_event.Broadcast(this, MakeShared<UE::Smith::Command::MoveCommand>(m_moveComp));
			MDebug::LogWarning(GetName() + TEXT("send move Command"));
		}
	}
}

UTurnControlComponent *AMyEnemy::GetTurnControl() const
{
	return m_turnCtrl;
}

FDelegateHandle AMyEnemy::Subscribe(FRequestCommandEvent::FDelegate &delegate)
{
	if (delegate.IsBound())
	{
		return m_event.Add(delegate);
	}

	return FDelegateHandle{};
}

bool AMyEnemy::Unsubscribe(UObject *obj, FDelegateHandle delegateHandle)
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

FVector AMyEnemy::MoveDirection()
{
	FVector myPos = GetActorLocation();
	FVector targetPos;
	FVector retPos = FVector::ZeroVector;
	if (m_target != nullptr)
	{
		targetPos = m_target->GetActorLocation();
	}

	if (targetPos.X > myPos.X)
	{
		myPos += MOVE_DISTANCE * FVector::ForwardVector;
	}
	else if (targetPos.X < myPos.X)
	{
		myPos += MOVE_DISTANCE * FVector::BackwardVector;
	}
	else if (targetPos.Y > myPos.Y)
	{
		myPos += MOVE_DISTANCE * FVector::RightVector;
	}
	else if (targetPos.Y < myPos.Y)
	{
		myPos += MOVE_DISTANCE * FVector::LeftVector;
	}

	MDebug::Log(myPos.ToString());
	retPos = myPos;

	return retPos;
}
