// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleMediator.h"
#include "SmithBattleSubsystem.h"
#include "SmithMapManager.h"

USmithBattleMediator::USmithBattleMediator()
  : m_battleSys(nullptr)
  , m_mapMgr(nullptr)
{

}

void USmithBattleMediator::BeginDestroy()
{
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

