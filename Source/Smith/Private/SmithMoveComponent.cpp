
#include "SmithMoveComponent.h"
#include "MyPlayerCharacter.h"
#include "SmithPlayerActor.h"
#include "Debug.h"
#include "Kismet/GameplayStatics.h"

USmithMoveComponent::USmithMoveComponent()
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
}

void USmithMoveComponent::Move()
{
	AActor *actor = GetOwner();
	if (actor != nullptr)
	{
		actor->SetActorLocation(m_terminus);
	}
	else 
	{
		MDebug::Log("Owner null No Move");
	}
}

void USmithMoveComponent::SetDestination(FVector destination)
{
	m_terminus = destination;
}
