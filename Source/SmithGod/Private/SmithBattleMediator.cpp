// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleMediator.h"
#include "SmithBattleSubsystem.h"
#include "SmithMapManager.h"

#include "IMoveable.h"
#include "ICanMakeAttack.h"
#include "IAttackable.h"
#include "ICanSetOnMap.h"

#include "MoveCommand.h"
#include "AttackCommand.h"
#include "NullCommand.h"

#include "SmithCommandFormat.h"
#include "FormatTransformer.h"
#include "Direction.h"
#include "ISmithAnimator.h"

#include "ISmithDamageCalculator.h"
#include "BattleParamHandle.h"
#include "BattleResult.h"

#include "InvalidValues.h"

#include "MLibrary.h"

// TODO
#include "AttackHandle.h"
#include "SmithSkillCenterSpotParameter.h"
#include "FormatType.h"

USmithBattleMediator::USmithBattleMediator()
  : m_battleSys(nullptr)
  , m_damageCalculator(nullptr)
  , m_mapMgr(nullptr)
{ }

void USmithBattleMediator::BeginDestroy()
{
  Super::BeginDestroy();
  m_battleSys.Reset();
  m_damageCalculator.Reset();
  m_mapMgr.Reset();
}

void USmithBattleMediator::SetupMediator(USmithBattleSubsystem* battleSys, ISmithDamageCalculator* damageCalculator, TSharedPtr<MapManager> mapMgr)
{
  check(((battleSys != nullptr) && (mapMgr != nullptr)));
  
  m_battleSys.Reset();
  m_battleSys = battleSys;

  m_mapMgr.Reset();
  m_mapMgr = mapMgr;

  m_damageCalculator.Reset();
  m_damageCalculator = damageCalculator;
  
}

bool USmithBattleMediator::SendMoveCommand(AActor* requester, IMoveable* move, EDirection moveDirection, uint8 moveDistance)
{
  if (!m_mapMgr.IsValid() || !m_battleSys.IsValid())
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  if (!::IsValid(requester) || !IS_UINTERFACE_VALID(move))
  {
    MDebug::LogError("Request INVALID!!!");
    // TODO
    return false;
  }

  FVector destinationVector;
  auto mapMgrSharedPtr = m_mapMgr.Pin();
  if (!mapMgrSharedPtr.IsValid())
  {
    return false;
  }

  mapMgrSharedPtr->MoveMapObj(Cast<ICanSetOnMap>(requester), moveDirection, moveDistance, destinationVector);

  if (destinationVector.Equals(InvalidValues::MapInvalidCoord_World))
  {
    return false;
  }
  else
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    move->SetDestination(destinationVector);
    m_battleSys->RegisterCommand(Cast<ITurnManageable>(requester), ::MakeShared<UE::Smith::Command::MoveCommand>(move, animator));
    return true;
  }
}

bool USmithBattleMediator::SendAttackCommand(AActor* requester, ICanMakeAttack* attacker, EDirection direction, const UE::Smith::Battle::FSmithCommandFormat& format, AttackHandle&& atkHandle, bool bAttackEvenNoTarget)
{
  if (!m_mapMgr.IsValid() || !m_battleSys.IsValid())
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  if (!::IsValid(requester))
  {
    MDebug::LogError("Requester INVALID!!!");
    return false;
  }

  if (format.GetRow() == 0 || format.GetColumn() == 0)
  {
    MDebug::LogError("Format INVALID");
    return false;
  }

  auto mapMgrSharedPtr = m_mapMgr.Pin();
  if (!mapMgrSharedPtr.IsValid())
  {
    return false;
  }

  // TODO
  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, direction);
  TArray<IAttackable*> attackables{};
  mapMgrSharedPtr->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat);

  // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(auto target : attackables)
    {
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, target, ::MoveTemp(atkHandle), animator));
    }
    return true;
  }

  if (bAttackEvenNoTarget)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, nullptr, ::MoveTemp(atkHandle), animator));
    return true;
  }
  else
  {
    return false;
  }
}

bool USmithBattleMediator::SendAttackCommand(AActor* requester, ICanMakeAttack* attacker, EDirection direction, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle, bool bAttackEvenNoTarget)
{
   if (!m_battleSys.IsValid())
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  if (!::IsValid(requester))
  {
    MDebug::LogError("Requester INVALID!!!");
    return false;
  }

  if (format.GetRow() == 0 || format.GetColumn() == 0)
  {
    MDebug::LogError("Format INVALID");
    return false;
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return false;
  }

  // TODO
  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, direction);
  TArray<IAttackable*> attackables{};
  mapMgr_shared->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat);

  // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(auto target : attackables)
    {
      // TODO 修正案ー＞BattleModelを作成し、Data Assetsで設定できるようにする
      AttackHandle attackHandle;
      attackHandle.Attacker = atkHandle.Attacker;
      if (target != nullptr && m_damageCalculator.IsValid())
      { 
        FBattleAttackParamHandle attackParam;
        attackParam.AttackPoint = atkHandle.AttackPower;
        attackParam.CriticalPoint = atkHandle.CriticalPower;
        attackParam.Level = atkHandle.Level;
        attackParam.MotionValue = atkHandle.MotionValue;

        const FBattleResult result = m_damageCalculator->CalculateDamage(attackParam, target->GetDefenseParam());
        attackHandle.AttackPower = result.Damage;
      }
      else
      {
        attackHandle.AttackPower = atkHandle.AttackPower;
      }
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, target, ::MoveTemp(attackHandle), animator));
    }
    return true;
  }

  if (bAttackEvenNoTarget)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    AttackHandle attackHandle = AttackHandle::NullHandle;
    m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, nullptr, ::MoveTemp(attackHandle), animator));
    return true;
  }
  else
  {
    return false;
  }
}

bool USmithBattleMediator::SendSkillCommand(AActor* requester, ICanMakeAttack* attacker, FSmithSkillCenterSpotParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle)
{
  if (!m_battleSys.IsValid())
  {
    return false;
  }

  if (!::IsValid(requester) || !IS_UINTERFACE_VALID(attacker))
  {
    return false;
  }

  if (format.GetRow() == 0 || format.GetColumn() == 0)
  {
    MDebug::LogError("Format INVALID");
    return false;
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return false;
  }

  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, skillParameter.Direction);
  TArray<IAttackable*> attackables{};
  mapMgr_shared->FindAttackableMapObjsFromCoord(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat, skillParameter.OffsetToLeft, skillParameter.OffsetToTop);

   // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(auto target : attackables)
    {
      // TODO 修正案ー＞BattleModelを作成し、Data Assetsで設定できるようにする
      AttackHandle attackHandle;
      attackHandle.Attacker = atkHandle.Attacker;
      if (target != nullptr && m_damageCalculator.IsValid())
      { 
        FBattleAttackParamHandle attackParam;
        attackParam.AttackPoint = atkHandle.AttackPower;
        attackParam.CriticalPoint = atkHandle.CriticalPower;
        attackParam.Level = atkHandle.Level;
        attackParam.MotionValue = atkHandle.MotionValue;

        const FBattleResult result = m_damageCalculator->CalculateDamage(attackParam, target->GetDefenseParam());
        attackHandle.AttackPower = result.Damage;
      }
      else
      {
        attackHandle.AttackPower = atkHandle.AttackPower;
      }
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, target, ::MoveTemp(attackHandle), animator));
    }
    return true;
  }

  return false;
}

bool USmithBattleMediator::SendIdleCommand(AActor* requester)
{
  if (!m_battleSys.IsValid())
  {
    return false;
  }
  
  if (!::IsValid(requester))
  {
    return false;
  }

  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (!IS_UINTERFACE_VALID(requesterTurnManageable))
  {
    return false;
  }

  m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::NullCommand>());
  return true;

}

int32 USmithBattleMediator::GetRangeLocations(TArray<FVector>& outLocations, AActor* requester, FSmithSkillCenterSpotParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format) const
{ 
  outLocations.Reset();

  if (!::IsValid(requester))
  {
    return outLocations.Num();
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return outLocations.Num();
  }

  if (format.GetRow() == 0 || format.GetColumn() == 0)
  {
    MDebug::LogError("Format INVALID");
    return outLocations.Num();
  }

  ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(requester);
  if (!IS_UINTERFACE_VALID(mapObj))
  {
    return outLocations.Num();
  }

  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, skillParameter.Direction);
  for (int32 y = 0; y < rotatedFormat.GetRow(); ++y)
  {
    for (int32 x = 0; x < rotatedFormat.GetColumn(); ++x)
    {
      switch(rotatedFormat.GetFormatData(x, y))
      {
        case ESmithFormatType::NO_EFFECT:
        case ESmithFormatType::CENTER_NO_EFFECT:
        {
          continue;
        }

        case ESmithFormatType::EFFECT:
        case ESmithFormatType::CENTER_EFFECT:
        {
          uint8 coordX = 0u;
          uint8 coordY = 0u;
          if (!mapMgr_shared->GetMapObjectCoord(mapObj, coordX, coordY))
          {
            continue;
          }

          FVector location;
          if (!mapMgr_shared->ConvertMapCoordToWorldLocation(location, coordX, coordY))
          {
            continue;
          }

          outLocations.Emplace(location);
        }
        break;
      }
    }
  }

  return outLocations.Num();
}

