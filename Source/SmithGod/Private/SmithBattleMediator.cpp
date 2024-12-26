// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleMediator.h"
#include "SmithBattleSubsystem.h"
#include "SmithMapManager.h"

#include "IMoveable.h"
#include "ICanMakeAttack.h"
#include "IAttackable.h"

#include "MoveCommand.h"
#include "AttackCommand.h"

#include "Debug.h"

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

void USmithBattleMediator::SendMoveCommand(AActor* requester, IMoveable* move)
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

  m_battleSys->registerCommand(Cast<ITurnManageable>(requester), ::MakeShared<UE::Smith::Command::MoveCommand>(move));

}

void USmithBattleMediator::SendAttackCommand(AActor* requester, ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& atkHandle)
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

  m_battleSys->registerCommand(Cast<ITurnManageable>(requester), ::MakeShared<UE::Smith::Command::AttackCommand>(attacker, target, ::MoveTemp(atkHandle)));
}

