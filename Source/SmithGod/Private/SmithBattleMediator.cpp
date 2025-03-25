// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleMediator.h"

#include "SmithBattleSubsystem.h"
#include "SmithDamageSubsystem.h"
#include "SmithCommandStrategies.h"
#include "SmithMapManager.h"

#include "ICanSetOnMap.h"
#include "BattleCommandFactory.h"

#include "SmithCommandFormat.h"
#include "FormatTransformer.h"
#include "ISmithAnimator.h"
#include "InvalidValues.h"
#include "MLibrary.h"
#include "AttackHandle.h"
#include "SmithSkillParameter.h"
#include "FormatType.h"
#include "AttackableInfoHandle.h"
#include "SmithModelHelperFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithBattleMediator)

USmithBattleMediator::USmithBattleMediator()
  : m_battleSys(nullptr)
  , m_damageSys(nullptr)
  , m_mapMgr(nullptr)
{ }

void USmithBattleMediator::BeginDestroy()
{
  Super::BeginDestroy();

  m_mapMgr.Reset();
}

void USmithBattleMediator::SetupMediator(USmithBattleSubsystem* BattleSystem, USmithDamageSubsystem* DamageSystem, TSharedPtr<MapManager> MapMgr)
{
  check(BattleSystem != nullptr);
  check(DamageSystem != nullptr);
  check(MapMgr != nullptr);
  
  m_battleSys = BattleSystem;
  m_damageSys = DamageSystem;

  m_mapMgr.Reset();
  m_mapMgr = MapMgr;
  
}

bool USmithBattleMediator::SendMoveCommand(AActor* requester, EDirection moveDirection, uint8 moveDistance)
{
  check(requester != nullptr);

  if (m_battleSys == nullptr)
  {
    MDebug::LogError("Battle System INVALID!!!");
    return false;
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    MDebug::LogError("Map Manager INVALID!!!");
    return false;
  }
  
  FVector destination = FVector::ZeroVector;
  mapMgr_shared->MoveMapObj(Cast<ICanSetOnMap>(requester), moveDirection, moveDistance, destination);

  if (destination.Equals(InvalidValues::MAP_COORD_WORLD_INVALID))
  {
    return false;
  }

  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    SmithDefaultMoveStrategy moveStrategy = 
    {
      .MoveEntity = requester,
      .MoveSpeed = 800.0f,        // TODO
      .Destination = destination
    };
    m_battleSys->RegisterCommand(Cast<ITurnManageable>(requester), FBattleCommandFactory::CreateMoveCommand(moveStrategy, animator));
  }

  return true;
}

bool USmithBattleMediator::SendAttackCommand(AActor* requester, EDirection direction, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle, bool bAttackEvenNoTarget)
{
  check(requester != nullptr);
  check(format.IsValid());

  if ((m_battleSys == nullptr) || (m_damageSys == nullptr))
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    MDebug::LogError("Map Manager INVALID");
    return false;
  }

  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, direction);
  TArray<FAttackableInfoHandle> attackables{};
  mapMgr_shared->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat);

  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (attackables.Num() > 0)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    for(const auto& attackableInfo : attackables)
    {
      SmithDefaultDamageStrategy damageStrategy{};
      damageStrategy.Instigator = requester;
      damageStrategy.Causer = Cast<AActor>(attackableInfo.Attackable);
      damageStrategy.DamageSubsystem = m_damageSys;
      damageStrategy.Handle = atkHandle;
      damageStrategy.FromDirection = attackableInfo.AttackFrom;

      m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateAttackCommand(damageStrategy, animator));
    }
    return true;
  }

  if (bAttackEvenNoTarget)
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    AttackHandle attackHandle = AttackHandle::NullHandle;
    SmithDummyStrategy dummy{};

    m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateAttackCommand(dummy, animator));
    return true;
  }
  else
  {
    return false;
  }
}

bool USmithBattleMediator::SendSkillCommand(AActor* requester, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle)
{
  check(requester != nullptr);
  check(format.IsValid());
  
  if ((m_battleSys == nullptr) || (m_damageSys == nullptr))
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return false;
  }

  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  TArray<FAttackableInfoHandle> attackables{};
  mapMgr_shared->FindAttackableMapObjsFromCoord(attackables, Cast<ICanSetOnMap>(requester), rotatedFormat, skillParameter.OffsetToLeft, skillParameter.OffsetToTop);
  
  // TODO Safe Cast may cause performance issue
  {
    ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    if (attackables.Num() > 0)
    {
      for(const auto& attackableInfo : attackables)
      {
        SmithDefaultDamageStrategy damageStrategy{};
        damageStrategy.Instigator = requester;
        damageStrategy.Causer = Cast<AActor>(attackableInfo.Attackable);
        damageStrategy.DamageSubsystem = m_damageSys;
        damageStrategy.Handle = atkHandle;
        damageStrategy.FromDirection = attackableInfo.AttackFrom;
  
        m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateSkillCommand(damageStrategy, animator, skillParameter.SkillSlot));
      }
      return true;
    }
    else
    {
      SmithDummyStrategy dummy{};
      m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateSkillCommand(dummy, animator, skillParameter.SkillSlot));
    }
  }

  return false;
}

bool USmithBattleMediator::SendIdleCommand(AActor* requester, float idleTime)
{
  check(requester != nullptr);

  if (m_battleSys == nullptr)
  {
    return false;
  }

  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (requesterTurnManageable == nullptr)
  {
    return false;
  }

  m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateIdleCommand(idleTime));

  return true;
}

int32 USmithBattleMediator::GetRangeLocations(TArray<FVector>& outLocations, AActor* requester, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format)
{ 
  using namespace UE::Smith::Battle;
  check(requester != nullptr);
  check(format.IsValid());

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return 0;
  }
  
  outLocations.Reset();

  ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(requester);
  uint8 mapObjOriginCoordX = 0u;
  uint8 mapObjOriginCoordY = 0u;
  if (!mapMgr_shared->GetMapObjectCoord(mapObj, mapObjOriginCoordX, mapObjOriginCoordY))
  {
    return 0;
  }

  FSmithCommandFormat rotatedFormat = FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  auto formattedMapCoords = FFormatTransformer::FormatToMapCoord(rotatedFormat, FMapCoord(mapObjOriginCoordX + skillParameter.OffsetToLeft, mapObjOriginCoordY + skillParameter.OffsetToTop));

  // フォーマットから効果範囲を座標に変換する
  for (int32 row = 0; row < rotatedFormat.GetRow(); ++row)
  {
    for (int32 column = 0; column < rotatedFormat.GetColumn(); ++column)
    {
      using enum ESmithFormatType;
      switch(rotatedFormat.GetFormatData(column, row))
      {
        case EFFECT:
        case CENTER_EFFECT:
        {
          const FMapCoord coord = formattedMapCoords.At_ReadOnly(row, column);

          FVector location = FVector::ZeroVector;
          if (!mapMgr_shared->ConvertMapCoordToWorldLocation(location, coord.x, coord.y))
          {
            continue;
          }

          outLocations.Emplace(location);
        }
        break;

        case NO_EFFECT:
        case CENTER_NO_EFFECT:
        default:
        {
          continue;
        }
      }
    }
  }

  return outLocations.Num();
}

void USmithBattleMediator::GetPlayerDirection(EDirection& outDirection, EDirectionStrategy directionStrategy, AActor* requester, uint8 offsetToLeft, uint8 offsetToTop)
{
  check(requester != nullptr);

  TSharedPtr<MapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return;
  }

  ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(requester);

  mapMgr_shared->GetPlayerDirection(outDirection, directionStrategy, mapObj, offsetToLeft, offsetToTop);
}

