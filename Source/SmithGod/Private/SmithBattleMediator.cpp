// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleMediator.h"
#include "SmithBattleSubsystem.h"
#include "SmithMapManager.h"

#include "IMoveable.h"
#include "ICanMakeAttack.h"
#include "ICanSetOnMap.h"
#include "IHealable.h"

#include "MoveCommand.h"
#include "AttackCommand.h"
#include "NullCommand.h"
#include "HealCommand.h"
#include "SkillCommand.h"

#include "SmithCommandFormat.h"
#include "FormatTransformer.h"
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
#include "AttackableInfoHandle.h"

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
  TArray<FAttackableInfoHandle> attackables{};
  mapMgr_shared->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat);

  // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(const auto& attackableInfoHandle : attackables)
    {
      atkHandle.AttackFrom = attackableInfoHandle.AttackFrom;
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, attackableInfoHandle.Attackable, ::MoveTemp(atkHandle), animator));
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
  TArray<FAttackableInfoHandle> attackables{};
  mapMgr_shared->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat);

  // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(const auto& attackableInfo : attackables)
    {
      // TODO 修正案ー＞BattleModelを作成し、Data Assetsで設定できるようにする
      AttackHandle attackHandle;
      attackHandle.Attacker = atkHandle.Attacker;
      if (attackableInfo.Attackable != nullptr && m_damageCalculator.IsValid())
      { 
        FBattleAttackParamHandle attackParam;
        attackParam.AttackPoint = atkHandle.AttackPower;
        attackParam.CriticalPoint = atkHandle.CriticalPower;
        attackParam.Level = atkHandle.Level;
        attackParam.MotionValue = atkHandle.MotionValue;

        const FBattleResult result = m_damageCalculator->CalculateDamage(attackParam, attackableInfo.Attackable->GetDefenseParam());
        attackHandle.AttackPower = result.Damage;
      }
      else
      {
        attackHandle.AttackPower = atkHandle.AttackPower;
      }

      attackHandle.AttackFrom = attackableInfo.AttackFrom;
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, attackableInfo.Attackable, ::MoveTemp(attackHandle), animator));
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

bool USmithBattleMediator::SendSkillCommand(AActor* requester, ICanMakeAttack* attacker, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle)
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

  // TODO
  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  TArray<FAttackableInfoHandle> attackables{};
  mapMgr_shared->FindAttackableMapObjsFromCoord(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat, skillParameter.OffsetToLeft, skillParameter.OffsetToTop);
  
   // TODO Safe Cast may cause performance issue
  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
  if (attackables.Num() > 0)
  {
    for(const auto& attackableInfo : attackables)
    {
      // TODO 修正案ー＞BattleModelを作成し、Data Assetsで設定できるようにする
      AttackHandle attackHandle;
      attackHandle.Attacker = atkHandle.Attacker;
      if (attackableInfo.Attackable != nullptr && m_damageCalculator.IsValid())
      { 
        FBattleAttackParamHandle attackParam;
        attackParam.AttackPoint = atkHandle.AttackPower;
        attackParam.CriticalPoint = atkHandle.CriticalPower;
        attackParam.Level = atkHandle.Level;
        attackParam.MotionValue = atkHandle.MotionValue;

        const FBattleResult result = m_damageCalculator->CalculateDamage(attackParam, attackableInfo.Attackable->GetDefenseParam());
        attackHandle.AttackPower = result.Damage;
      }
      else
      {
        attackHandle.AttackPower = atkHandle.AttackPower;
      }

      attackHandle.AttackFrom = attackableInfo.AttackFrom;
      m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::SkillCommand>(attacker, attackableInfo.Attackable, ::MoveTemp(attackHandle), animator, skillParameter.SkillSlot));
    }
    return true;
  }
  else
  {
    m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::SkillCommand>(nullptr, nullptr, AttackHandle::NullHandle, animator, skillParameter.SkillSlot));
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

bool USmithBattleMediator::SendHealCommand(AActor* requester,IHealable* heal)
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

  m_battleSys->RegisterCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::HealCommand>(heal));
  return true;

}

// TODO 悪い参照渡し 
int32 USmithBattleMediator::GetRangeLocations(TArray<FVector>& outLocations, AActor* requester, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format) const
{ 
  using namespace UE::Smith::Battle;
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

  uint8 mapObjOriginCoordX = 0u;
  uint8 mapObjOriginCoordY = 0u;
  if (!mapMgr_shared->GetMapObjectCoord(mapObj, mapObjOriginCoordX, mapObjOriginCoordY))
  {
    return outLocations.Num();
  }

  FSmithCommandFormat rotatedFormat = FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  auto formattedMapCoords = FFormatTransformer::FormatToMapCoord(rotatedFormat, FMapCoord(mapObjOriginCoordX + skillParameter.OffsetToLeft, mapObjOriginCoordY + skillParameter.OffsetToTop));

  for (int32 row = 0; row < rotatedFormat.GetRow(); ++row)
  {
    for (int32 column = 0; column < rotatedFormat.GetColumn(); ++column)
    {
      // TODO
      switch(rotatedFormat.GetFormatData(column, row))
      {
        case ESmithFormatType::NO_EFFECT:
        case ESmithFormatType::CENTER_NO_EFFECT:
        {
          continue;
        }

        case ESmithFormatType::EFFECT:
        case ESmithFormatType::CENTER_EFFECT:
        {
          const FMapCoord coord = formattedMapCoords.At_ReadOnly(row, column);

          FVector location;
          if (!mapMgr_shared->ConvertMapCoordToWorldLocation(location, coord.x, coord.y))
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

void USmithBattleMediator::GetPlayerDirection(EDirection& outDirection, AActor* requester, uint8 offsetToLeft, uint8 offsetToTop)
{
  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return;
  }

  mapMgr_shared->GetPlayerDirection(outDirection, Cast<ICanSetOnMap>(requester), offsetToLeft, offsetToTop);
}

