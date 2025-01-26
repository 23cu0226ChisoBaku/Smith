// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"
#include "AttackHandle.h"
#include "SmithAIRegistry.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"
#include "SmithAttackComponent.h"
#include "SmithMoveComponent.h"

#include "SmithAnimationComponent.h"

#include "FormatInfo_Import.h"
#include "SmithMoveDirector.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "MLibrary.h"

ATurnActor_Test::ATurnActor_Test()
	: m_attackStrategy(nullptr)
	, m_moveStrategy(nullptr)
	, m_idleStrategy(nullptr)
	, m_atkComponent(nullptr)
	, MoveComponent(nullptr)
	, AnimComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	SetTurnPriority(ETurnPriority::Rival);

	m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("attack comp test"));
	check(m_atkComponent != nullptr);

	MoveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("move comp test"));
	check(MoveComponent != nullptr);

	AnimComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("anim comp"));
	check(AnimComponent != nullptr)

}

void ATurnActor_Test::BeginPlay()
{
	Super::BeginPlay();

	m_attackStrategy = NewObject<USmithTurnBaseAIAttackStrategy>(this);
	check(m_attackStrategy != nullptr);
	m_moveStrategy = NewObject<USmithTurnBaseAIMoveStrategy>(this);
	check(m_moveStrategy != nullptr);
	m_idleStrategy = NewObject<USmithTurnBaseAIIdleStrategy>(this);
	check(m_idleStrategy != nullptr);
}

void ATurnActor_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->StopBehaviorProcessor();
		m_aiBehaviorProcessor->MarkAsGarbage();
	}
}

void ATurnActor_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->TickBehaviorProcessor(DeltaTime);
	}
}

void ATurnActor_Test::OnAttack(AttackHandle&& handle)
{
	EnemyParam.HP -= handle.AttackPower;

	MDebug::LogWarning(GetName() + " left HP:" + FString::FromInt(EnemyParam.HP));
	if (EnemyParam.HP <= 0)
	{
		MDebug::LogError(GetName() + " Dead");
		if (m_eventMediator.IsValid())
		{
			if (DropUpgradeTable.Num() > 0)
			{
				m_eventMediator->PublishPickUpEvent(this, DropUpgradeTable[0]);
				DropUpgradeTable.RemoveAt(0);
			}
		}
		Destroy();
		DropUpgradeTable.Reset();
	}
}

uint8 ATurnActor_Test::GetOnMapSizeX() const
{
	return 1;
}

uint8 ATurnActor_Test::GetOnMapSizeY() const
{
	return 1;
}

UClass* ATurnActor_Test::GetMoveDirectorUClass() const
{
	return MoveDirectorSubclass.Get();
}

void ATurnActor_Test::SetMoveDirector(USmithMoveDirector* director)
{
	m_moveDirector = director;
}

uint8 ATurnActor_Test::GetChaseRadius() const
{
	return ChaseRadius;
}

EMapObjType ATurnActor_Test::GetType() const
{
	return MapObjectType;
}

void ATurnActor_Test::TurnOnAI()
{
	if (m_attackStrategy != nullptr)
	{
		m_attackStrategy->SetOwner(this);
		m_attackStrategy->Initialize(m_atkComponent, m_commandMediator.Get(), EnemyParam.ATK);
	}

	for (auto& pair : AttackFormatTables)
	{
		if (!pair.Value.IsValid())
		{
			pair.Value.LoadSynchronous();
		}

		if (m_attackStrategy != nullptr)
		{
			m_attackStrategy->RegisterAttackFormat(pair.Key, pair.Value.Get());
		}
	}

	if (m_moveStrategy != nullptr)
	{
		m_moveStrategy->SetOwner(this);
		m_moveStrategy->Initialize(m_commandMediator.Get(), m_moveDirector, MoveComponent, 1);
	}

	if (m_idleStrategy != nullptr)
	{
		m_idleStrategy->SetOwner(this);
		m_idleStrategy->Initialize(m_commandMediator.Get());
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->RegisterAIStrategy(0, m_attackStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(1, m_moveStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(2, m_idleStrategy);
		m_aiBehaviorProcessor->RunBehaviorProcessor();
	}
}

void ATurnActor_Test::SetEventPublishMediator(IEventPublishMediator* eventMediator)
{
	m_eventMediator = eventMediator;
}

void ATurnActor_Test::SwitchAnimation(uint8 animationState)
{
	//MDebug::Log(TEXT("called animation"));

	if (AnimComponent == nullptr)
	{
		return;
	}

	using namespace UE::Smith;
	FName StateName;
	switch (animationState)
	{
	case SMITH_ANIM_IDLE:
		StateName = TEXT("Idle");
		break;
	case	SMITH_ANIM_WALK:
		StateName = TEXT("Walk");
		break;
	case SMITH_ANIM_ATTACK:
		StateName = TEXT("Attack");
		break;
	case SMITH_ANIM_DAMAGED:
		StateName = TEXT("Damaged");
		break;
	case SMITH_ANIM_DEAD:
		StateName = TEXT("Dead");
		break;
	default:
		break;
	}
	AnimComponent->SwitchAnimState(StateName, 0.0f);
}

void ATurnActor_Test::UpdateAnimation(float deltaTime)
{
	AnimComponent->UpdateAnim(deltaTime);
}

bool ATurnActor_Test::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
}
