
#include "SmithMoveComponent.h"
#include "SmithPlayerActor.h"
#include "Debug.h"
#include "Kismet/GameplayStatics.h"

USmithMoveComponent::USmithMoveComponent()
	: m_terminus(FVector::ZeroVector)
	, m_bCanMove(false)
{
}

void USmithMoveComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 終点をセット
void USmithMoveComponent::SetTerminusPos(FVector pos)
{
	m_terminus = pos;
	m_bCanMove = true;
}

void USmithMoveComponent::Move(float deltaTime)
{
	if (!m_bCanMove)
	{
		return;
	}

	AActor *actor = GetOwner();
	if (actor == nullptr)
	{
		return;
	}

	const FVector curtLocation = actor->GetActorLocation();
	const FVector leftDistanceVec = m_terminus - curtLocation;
	FVector curtMovement = leftDistanceVec.GetSafeNormal() * deltaTime * MoveSpeed;
	if (curtMovement.SquaredLength() > leftDistanceVec.SquaredLength())
	{
		curtMovement = leftDistanceVec;
		m_bCanMove = false;
	}

	const FVector nextLocation = curtLocation + curtMovement;
	actor->SetActorLocation(nextLocation);
}

void USmithMoveComponent::SetDestination(FVector destination)
{
	m_terminus = destination;
	m_bCanMove = true;
}

bool USmithMoveComponent::IsReachDestination() const
{
	AActor* actor = GetOwner();
	return ::IsValid(actor) ? actor->GetActorLocation().Equals(m_terminus) : false;
	
}
