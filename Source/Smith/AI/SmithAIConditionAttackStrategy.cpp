// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAIConditionAttackStrategy.h"
#include "ICommandMediator.h"
#include "AttackHandle.h"
#include "Direction.h"
#include "MLibrary.h"

#include "SmithModelHelperFunctionLibrary.h"

#include "SmithDangerZoneDisplayer.h"

USmithAIConditionAttackStrategy::USmithAIConditionAttackStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_atk(0)
  , m_crt(0)
  , m_level(1)
  , m_curtConditionCooldown(0)
  , m_bIsDisplayingDangerZone(false)
  , m_bIsWaitCondition(false)
{ }

void USmithAIConditionAttackStrategy::BeginDestroy()
{
  if (m_dangerZoneDisplayer != nullptr)
  {
    m_dangerZoneDisplayer->Dispose();
  }
  
  Super::BeginDestroy();
}

void USmithAIConditionAttackStrategy::Initialize(ICommandMediator *mediator, int32 attackPower)
{
  m_mediator = mediator;
  m_atk = attackPower;

  AActor* owner = GetOwner();
  if (!::IsValid(owner))
  {
    return;
  }

  // TODO need ignore hard coding
  UWorld* world = owner->GetWorld();
  if (::IsValid(world))
  {
    TSubclassOf<ASmithDangerZoneDisplayer> dangerZoneSub = TSoftClassPtr<ASmithDangerZoneDisplayer>(FSoftObjectPath("/Game/BP/BP_SmithDangerZoneDisplayer.BP_SmithDangerZoneDisplayer_C")).LoadSynchronous();
    if (dangerZoneSub != nullptr)
    {
      m_dangerZoneDisplayer = world->SpawnActor<ASmithDangerZoneDisplayer>(dangerZoneSub, owner->GetActorLocation(), owner->GetActorRotation());
    }
  }
}

void USmithAIConditionAttackStrategy::SetAttackParam(int32 attackPower, int32 critical, int32 level)
{
  m_atk = attackPower;
  m_crt = critical;
  m_level = level;
}

void USmithAIConditionAttackStrategy::ConditionResgister(const FString &name, const UDataTable *formatTable, const TDelegate<bool(void)> &condition, FSmithSkillParameter skillParameter)
{
  if (condition.IsBound())
  {
    RegisterAttackFormat(name, formatTable);
    FConditionHandle handle;
    handle.Name = name;
    handle.Condition = condition;
    handle.SkillParameter = skillParameter;
    m_conditions.Enqueue(handle);
  }
  else
  {
    MDebug::LogError("Condition is not bound");
  }
}

bool USmithAIConditionAttackStrategy::executeImpl()
{
  if (!m_mediator.IsValid())
  {
    MDebug::LogError("not initialize -- attack strategy");
    return false;
  }

  if (m_curtConditionCooldown > 0)
  {
    --m_curtConditionCooldown;
    return false;
  }

  FConditionHandle* curtConditionAttatkHandle = m_conditions.Peek();
  if (curtConditionAttatkHandle == nullptr)
  {
    return false;
  }

  if (!m_bIsWaitCondition)
  {
    m_bIsWaitCondition = true;
    EDirection playerDirection;
    AActor* owner = GetOwner();
    // TODO 設計を見直し
    m_mediator->GetPlayerDirection(playerDirection, EDirectionPolicy::Cardinal, GetOwner(), curtConditionAttatkHandle->SkillParameter.OffsetToLeft, curtConditionAttatkHandle->SkillParameter.OffsetToTop);
    curtConditionAttatkHandle->SkillParameter.ActiveDirection = playerDirection;
  
    if (playerDirection != EDirection::Invalid)
    {
      // TODO
      uint8 directionNum = StaticCast<uint8>(playerDirection);
      if (directionNum % 2 != 0)
      {
        directionNum -= 1;
      }

      if (::IsValid(owner))
      {
        const double newYaw = StaticCast<double>(directionNum) * 360.0 / StaticCast<double>(EDirection::DirectionCount);
        owner->SetActorRotation(FRotator{0.0, newYaw, 0.0});
      }
    }

    if (!m_bIsDisplayingDangerZone && m_dangerZoneDisplayer != nullptr)
    {
      const TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>& format = m_attackFormatTables[curtConditionAttatkHandle->Name];
      TArray<FVector> dangerZoneDisplayLocations;
      int32 num = m_mediator->GetRangeLocations(dangerZoneDisplayLocations, GetOwner(), curtConditionAttatkHandle->SkillParameter, *format);
  
      if (num > 0)
      {
        m_dangerZoneDisplayer->SetupDisplayLocations(dangerZoneDisplayLocations);
        m_bIsDisplayingDangerZone = true;
      }
    }
  }

  // 条件を満たしているか
  if (curtConditionAttatkHandle->Condition.Execute())
  {
    m_curtConditionCooldown = curtConditionAttatkHandle->SkillParameter.CooldownAfterUse;
    m_bIsWaitCondition = false;
    FConditionHandle curtHandleInstance{};
    m_conditions.Dequeue(curtHandleInstance);
    // 攻撃
    if (!m_attackFormatTables.Contains(curtHandleInstance.Name))
    {
      MDebug::LogError("Not Found Key");
      m_conditions.Enqueue(curtHandleInstance);
      return false;
    }

    const TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>& format = m_attackFormatTables[curtHandleInstance.Name];
    if (!format.IsValid())
    {
      MDebug::LogError("Format Invalid");
      m_conditions.Enqueue(curtHandleInstance);
      return false;
    }

    FAttackHandle handle;
    handle.AttackPower = m_atk;
    handle.Level = m_level;
    handle.MotionValue = curtHandleInstance.SkillParameter.MotionValue;

    bool success = m_mediator->SendSkillCommand(GetOwner(), curtHandleInstance.SkillParameter, *format, handle);
    m_conditions.Enqueue(curtHandleInstance);

    // TODO
    if (m_dangerZoneDisplayer != nullptr && m_bIsDisplayingDangerZone)
    {
      m_bIsDisplayingDangerZone = false;
      m_dangerZoneDisplayer->Dispose();
    }

    return success;
  }

  return false;
}