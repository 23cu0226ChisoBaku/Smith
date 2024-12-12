// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBaseActor.h"
#include "TurnControlComponent.h"

// Sets default values
ATurnBaseActor::ATurnBaseActor()
	: TurnComponent(nullptr)
	, m_event({})
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UTurnControlComponent* turnComp = CreateDefaultSubobject<UTurnControlComponent>(TEXT("TurnComponent"));

	if (turnComp != nullptr)
	{
		AddInstanceComponent(turnComp);
	}

	TurnComponent = turnComp;
}

// Called when the game starts or when spawned
void ATurnBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurnBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UTurnControlComponent *ATurnBaseActor::GetTurnControl() const
{
	check(TurnComponent != nullptr);

  return TurnComponent;
}

FDelegateHandle ATurnBaseActor::Subscribe(FRequestCommandEvent::FDelegate& delegate)
{
	if (delegate.IsBound())
	{
		return m_event.Add(delegate);
	}

	return delegate.GetHandle();
}

bool ATurnBaseActor::Unsubscribe(UObject* objPtr, FDelegateHandle handle)
{
	if (m_event.IsBoundToObject(objPtr))
	{
		m_event.Remove(handle);
		return true;
	}
	else
	{
		return false;
	}
}

void ATurnBaseActor::SendCommand(IBattleCommand* command)
{
	if (m_event.IsBound())
	{
		m_event.Broadcast(command);
	}
}
