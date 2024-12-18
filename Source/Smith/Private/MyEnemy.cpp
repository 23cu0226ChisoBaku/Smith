#include "MyEnemy.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TurnControlComponent.h"
#include "MoveCommand.h"

#include "Debug.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UTurnControlComponent* turnComp = CreateDefaultSubobject<UTurnControlComponent>(TEXT("Konno Enemy Turn CTRL"));

	if (turnComp != nullptr)
	{
		AddInstanceComponent(turnComp);
	}

	m_turnCtrl = turnComp;

	m_turnCtrl->SetTurnPriority(ETurnPriority::Rival);

	USmithMoveComponent* moveComp = CreateDefaultSubobject<USmithMoveComponent>(TEXT("konno asdhsaidhsaid"));

	if (moveComp)
	{
		AddInstanceComponent(moveComp);
	}

	m_moveComp = moveComp;

	m_moveComp->SetMoveSpeed(250.0f);
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	// コンポーネントの取得
	//m_attackComp = this->GetComponentByClass(USmithAttackComponent::StaticClass());
	//m_moveComp = Cast<USmithMoveComponent>(this->GetComponentByClass(USmithMoveComponent::StaticClass()));
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
		if (::IsValid(HitActor) && HitActor->IsA(AMyPlayerCharacter::StaticClass()))
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

			AMyPlayerCharacter *player = Cast<AMyPlayerCharacter>(HitActor);
			USmithAttackComponent *comp = Cast<USmithAttackComponent>(m_attackComp);
			comp->Attack(player, 1);
			return;
		}
	}

	// レイのデバッグ
	for (int i = 0; i < 4; ++i)
	{
		DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);

		DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
	}

	// 移動の処理

	if (m_event.IsBound())
	{
		m_event.Broadcast(Cast<ITurnManageable>(this), MakeShared<UE::Smith::Command::MoveCommand>(Cast<IMoveable>(m_moveComp)));
	}
	// USmithMoveComponent *comp = Cast<USmithMoveComponent>(m_moveComp);
	// comp->Move();
}

UTurnControlComponent* AMyEnemy::GetTurnControl() const
{
	return m_turnCtrl;
}

FDelegateHandle AMyEnemy::Subscribe(FRequestCommandEvent::FDelegate& delegate)
{
	if (delegate.IsBound())
	{
		return m_event.Add(delegate);
	}

	return FDelegateHandle{};
}

bool AMyEnemy::Unsubscribe(UObject* obj, FDelegateHandle delegateHandle)
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


