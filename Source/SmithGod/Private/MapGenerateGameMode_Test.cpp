// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerateGameMode_Test.h"
#include "SmithRect.h"
#include "SmithMap.h"
#include "SmithMapBuilder.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructor.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithMapManager.h"

#include "ICanCommandMediate.h"
#include "ICanSetOnMap.h"
#include "TurnActor_Test.h"
#include "SmithBattleMediator.h"
#include "SmithBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "SmithPlayerActor.h"

#include "MLibrary.h"

AMapGenerateGameMode_Test::AMapGenerateGameMode_Test()
  : m_battleMediator(nullptr)
  , m_mapMgr(nullptr)
{
  static ConstructorHelpers::FClassFinder<APawn> PlayerBP(TEXT("/Game/BP/BP_TestMapGeneratePawn"));
  if (PlayerBP.Class != nullptr)
  {
    DefaultPawnClass = PlayerBP.Class;
  }
}

void AMapGenerateGameMode_Test::StartPlay()
{
  Super::StartPlay();

  m_battleMediator = NewObject<USmithBattleMediator>();
  check((m_battleMediator != nullptr));

  // バトルシステム初期化
  USmithBattleSubsystem* subsys = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
  if (subsys != nullptr)
  {
    subsys->StartBattle();
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Can not init Battle System"));
    return;
  }

  // マップマネージャーを初期化
  m_mapMgr = ::MakeShared<UE::Smith::Map::FSmithMapManager>();

  m_battleMediator->SetupMediator(subsys, m_mapMgr);

  using namespace UE::Smith::Map;
  {
    m_mapMgr->InitMap(GetWorld(), MapBluePrint, MapConstructionBluePrint);
  }

  TArray<AActor*> canCmdMediateObjs;
  UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UCanCommandMediate::StaticClass(), canCmdMediateObjs);

  if (canCmdMediateObjs.Num() > 0)
  {
    for (auto obj : canCmdMediateObjs)
    {
      // TODO componentをInterfaceに変換
      ICanCommandMediate* mediatable = Cast<ICanCommandMediate>(obj);
      mediatable->SetCommandMediator(m_battleMediator);
    }
  }

  // TODO 
  // single playなので０番がプレイヤー
  APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  m_mapMgr->InitMapObjs(GetWorld(), playerPawn, EnemyGenerateBluePrint);

}

void AMapGenerateGameMode_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}