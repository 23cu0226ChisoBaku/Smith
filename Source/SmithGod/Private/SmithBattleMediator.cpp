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
#include "MoveDirection.h"

#include "InvalidValues.h"

#include "MLibrary.h"

USmithBattleMediator::USmithBattleMediator()
  : m_battleSys(nullptr)
  , m_mapMgr(nullptr)
{ }

void USmithBattleMediator::BeginDestroy()
{
  Super::BeginDestroy();
  m_battleSys.Reset();
  m_mapMgr.Reset();
}

void USmithBattleMediator::SetupMediator(USmithBattleSubsystem* battleSys, TSharedPtr<MapManager> mapMgr)
{
  check(((battleSys != nullptr) && (mapMgr != nullptr)));
  
  m_battleSys.Reset();
  m_battleSys = battleSys;

  m_mapMgr.Reset();
  m_mapMgr = mapMgr;
  
}

void USmithBattleMediator::SendMoveCommand(AActor* requester, IMoveable* move, UE::Smith::Battle::EMoveDirection moveDirection, uint8 moveDistance)
{
  if (!m_mapMgr.IsValid() || !m_battleSys.IsValid())
  {
    MDebug::LogError("System INVALID!!!");
    return;
  }

  if (!::IsValid(requester) || !IS_UINTERFACE_VALID(move))
  {
    MDebug::LogError("Request INVALID!!!");
    // TODO
    m_battleSys->registerCommand(Cast<ITurnManageable>(requester), ::MakeShared<UE::Smith::Command::NullCommand>());
    return;
  }

  FVector destinationVector;
  auto mapMgrSharedPtr = m_mapMgr.Pin();
  if (mapMgrSharedPtr.IsValid())
  {
    mapMgrSharedPtr->MoveMapObj(Cast<ICanSetOnMap>(requester), moveDirection, moveDistance, destinationVector);

    if (destinationVector.Equals(InvalidValues::MapInvalidCoord))
    {
      return;
    }
    else
    {
      move->SetDestination(destinationVector);
      m_battleSys->registerCommand(Cast<ITurnManageable>(requester), ::MakeShared<UE::Smith::Command::MoveCommand>(move));
    }
  }

}

void USmithBattleMediator::SendAttackCommand(AActor* requester, ICanMakeAttack* attacker, const UE::Smith::Battle::FSmithCommandFormat& format, AttackHandle&& atkHandle)
{
  if (!::IsValid(requester))
  {
    MDebug::LogError("Requester INVALID!!!");
    return;
  }

  if (!m_mapMgr.IsValid() || !m_battleSys.IsValid())
  {
    MDebug::LogError("System INVALID!!!");
    return;
  }

  auto mapMgrSharedPtr = m_mapMgr.Pin();
  if (mapMgrSharedPtr.IsValid())
  {
    TArray<IAttackable*> attackables{};
    // TODO Safe Cast may cause performance issue
    mapMgrSharedPtr->FindAttackableMapObjs(attackables, Cast<ICanSetOnMap>(requester), format);

    ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
    if (attackables.Num() > 0)
    {
      MDebug::LogError("found attack target");
      for(auto target : attackables)
      {
        // TODO Safe Cast may cause performance issue
        m_battleSys->registerCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, target, ::MoveTemp(atkHandle)));
      }
    }
    else
    {
      MDebug::LogError("Not found attack target");
      m_battleSys->registerCommand(requesterTurnManageable, ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, nullptr, ::MoveTemp(atkHandle)));
    }
  }

}

