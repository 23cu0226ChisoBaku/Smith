// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallGolem.h"

#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"

#include "SmithAnimationComponent.h"

#include "FormatInfo_Import.h"
#include "SmithMoveDirector.h"
#include "Direction.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "MLibrary.h"

#include "SmithEnemyParamInitializer.h"
#include "SmithEnemyLootGenerator.h"

#include "BattleParamHandle.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmallGolem)

ASmallGolem::ASmallGolem()
	: m_attackStrategy(nullptr)
	, m_moveStrategy(nullptr)
	, m_idleStrategy(nullptr)
	, AnimComponent(nullptr)
	, m_level(1)
	, m_bIsPlayingDeadAnimation(false)
	, m_bIsPlayingDamagedAnimation(false)
{
	PrimaryActorTick.bCanEverTick = true;
	SetTurnPriority(ETurnPriority::Rival);

	AnimComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("anim comp"));
	check(AnimComponent != nullptr)

}

void ASmallGolem::BeginPlay()
{
	Super::BeginPlay();

	m_attackStrategy = NewObject<USmithTurnBaseAIAttackStrategy>(this);
	check(m_attackStrategy != nullptr);
	m_moveStrategy = NewObject<USmithTurnBaseAIMoveStrategy>(this);
	check(m_moveStrategy != nullptr);
	m_idleStrategy = NewObject<USmithTurnBaseAIIdleStrategy>(this);
	check(m_idleStrategy != nullptr);

  AnimComponent->SwitchAnimState(TEXT("Idle"));
}

void ASmallGolem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->StopBehaviorProcessor();
		m_aiBehaviorProcessor->MarkAsGarbage();
	}
}

void ASmallGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_bIsPlayingDeadAnimation)
	{
		if (AnimComponent != nullptr && AnimComponent->IsCurrentAnimationFinish())
		{
			SetActorTickEnabled(false);
			Destroy();
		}
		AnimComponent->UpdateAnim(DeltaTime);

		return;
	}

	if (m_bIsPlayingDamagedAnimation)
	{
		if (AnimComponent != nullptr && AnimComponent->IsCurrentAnimationFinish())
		{
			m_bIsPlayingDamagedAnimation = false;
		}
		AnimComponent->UpdateAnim(DeltaTime);

		return;
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->TickBehaviorProcessor(DeltaTime);
	}
}

void ASmallGolem::OnAttack(const FBattleResult& Result)
{
	if (Result.Damage > 0)
	{
		EnemyParam.HP -= Result.Damage;

		if (Result.DamageFrom != EDirection::Invalid)
		{
			const EDirection faceTo = StaticCast<EDirection>((StaticCast<uint8>(Result.DamageFrom) + 4u) % StaticCast<uint8>(EDirection::DirectionCount)); 
			faceToDirection(faceTo);
		}

		if (AnimComponent != nullptr)
		{
			AnimComponent->SwitchAnimState(TEXT("Damaged"));
			m_bIsPlayingDamagedAnimation = true;
		}
	}

}

bool ASmallGolem::IsDefeated() const
{
	return EnemyParam.HP <= 0;
}	

void ASmallGolem::OnDefeated()
{
	// TODO Need Drop Item System
	if (m_eventMediator.IsValid())
	{
		IPickable* pickable = USmithEnemyLootGenerator::GetLoot(this);
		if (pickable != nullptr)
		{
			m_eventMediator->PublishPickUpEvent(this, pickable);
		}
	}

	if (OnDefeatEvent.IsBound())
	{
		OnDefeatEvent.Broadcast();
	}

	if (AnimComponent != nullptr)
	{
		AnimComponent->SwitchAnimState(TEXT("Dead"));
		m_bIsPlayingDeadAnimation = true;
	}
}

UClass* ASmallGolem::GetMoveDirectorUClass() const
{
	return MoveDirectorSubclass.Get();
}

void ASmallGolem::SetMoveDirector(USmithMoveDirector* director)
{
	m_moveDirector = director;
}

uint8 ASmallGolem::GetChaseRadius() const
{
	return ChaseRadius;
}

void ASmallGolem::TurnOnAI()
{
	if (m_attackStrategy != nullptr)
	{
		m_attackStrategy->SetOwner(this);
		m_attackStrategy->Initialize(m_commandMediator.Get(), EnemyParam.ATK);
		m_attackStrategy->SetAttackParam(EnemyParam.ATK, EnemyParam.CRT, m_level);
		m_attackStrategy->OnChangeDirectionDelegate.BindUObject(this, &ThisClass::faceToDirection);
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
		m_moveStrategy->Initialize(m_commandMediator.Get(), m_moveDirector, 1);
		m_moveStrategy->OnMoveToEvent.BindUObject(this, &ThisClass::faceToDirection);
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

void ASmallGolem::SetEventPublishMediator(IEventPublishMediator* eventMediator)
{
	m_eventMediator = eventMediator;
}

void ASmallGolem::SwitchAnimation(uint8 animationState)
{
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
	AnimComponent->SwitchAnimState(StateName);
}

void ASmallGolem::UpdateAnimation(float deltaTime)
{
	AnimComponent->UpdateAnim(deltaTime);
}

bool ASmallGolem::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
}

void ASmallGolem::InitializeParameter(int32 currentLevel)
{
	EnemyParam = USmithEnemyParamInitializer::GetParams(*this, currentLevel);
	// TODO
	m_level = 1 + (currentLevel - 1) * 3;
}

void ASmallGolem::faceToDirection(EDirection newDirection)
{
	if (StaticCast<uint8>(newDirection) >= StaticCast<uint8>(EDirection::DirectionCount))
	{
		return;
	}

	const double newYaw = StaticCast<double>(newDirection) * 360.0 / StaticCast<double>(EDirection::DirectionCount);
	SetActorRotation(FRotator{0.0, newYaw, 0.0});
}

FBattleDefenseParamHandle ASmallGolem::GetDefenseParam() const
{
	FBattleDefenseParamHandle handle;
	handle.DefensePoint = EnemyParam.DEF;
	handle.Level = m_level;
	return handle;
}
