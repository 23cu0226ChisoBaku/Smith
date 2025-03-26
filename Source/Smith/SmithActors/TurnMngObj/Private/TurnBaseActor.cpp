// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBaseActor.h"
#include "ICommandMediator.h"
#include "Direction.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategy.h"

#include "MLibrary.h"

// Sets default values
ATurnBaseActor::ATurnBaseActor()
	: m_aiBehaviorProcessor(nullptr)
	, m_commandMediator(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnBaseActor::BeginPlay()
{
	Super::BeginPlay();

	if (bUseSmithAIProcessor)
	{
		m_aiBehaviorProcessor = NewObject<USmithAIBehaviorProcessor>(this);
		check(m_aiBehaviorProcessor != nullptr);

		m_aiBehaviorProcessor->TickConditionDelegate.BindUObject(this, &ATurnBaseActor::IsCommandSendable);
	}

}

// Called every frame
void ATurnBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurnBaseActor::SetCommandMediator(ICommandMediator* mediator)
{
	m_commandMediator = mediator;
}

void ATurnBaseActor::InitializeParameter(int32 currentLevel)
{
	
}

UTexture2D* ATurnBaseActor::GetMinimapDisplayTexture_Implementation()
{
	return MinimapTexture;
}