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
#include "IMoveDirector.h"
#include "TurnActor_Test.h"
#include "SmithBattleMediator.h"
#include "SmithChasePlayerTracker.h"
#include "SmithBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "SmithPlayerActor.h"
#include "SmithMapBaseMoveDirector.h"

#include "MLibrary.h"

AMapGenerateGameMode_Test::AMapGenerateGameMode_Test()
  : m_battleMediator(nullptr)
  , m_chasePlayerTracker(nullptr)
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
  m_battleMediator = NewObject<USmithBattleMediator>(this);
  check((m_battleMediator != nullptr));

  // マップマネージャーを初期化
  m_mapMgr = ::MakeShared<UE::Smith::Map::FSmithMapManager>();
  check(m_mapMgr.IsValid());

  m_mapMgr->InitMap(GetWorld(), MapBluePrint, MapConstructionBluePrint);
  using namespace UE::Smith::Map;

  // TODO 
  // single playなので０番がプレイヤー
  APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  m_mapMgr->InitMapObjs(GetWorld(), playerPawn, EnemyGenerateBluePrint);

  {
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
    m_battleMediator->SetupMediator(subsys, m_mapMgr);
  }

  {
    TArray<AActor*> canCmdMediateObjs;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UCanCommandMediate::StaticClass(), canCmdMediateObjs);

    if (canCmdMediateObjs.Num() > 0)
    {
      for (auto& obj : canCmdMediateObjs)
      {
        // TODO componentをInterfaceに変換
        ICanCommandMediate* mediatable = Cast<ICanCommandMediate>(obj);
        mediatable->SetCommandMediator(m_battleMediator);
      }
    }
  }

  {
    TArray<AActor*> moveDirectorImplementedActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UMoveDirector::StaticClass(), moveDirectorImplementedActors);
    {
      if (moveDirectorImplementedActors.Num() > 0)
      {
        m_chasePlayerTracker = NewObject<USmithChasePlayerTracker>(this);
        check(m_chasePlayerTracker != nullptr);

        for (auto& obj : moveDirectorImplementedActors)
        {
          ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(obj);
          if (mapObj == nullptr)
          {
            continue;
          }

          IMoveDirector* moveDirector = Cast<IMoveDirector>(obj);
          UClass* moveDirectorSub = moveDirector->GetMoveDirectorUClass();

          if (moveDirectorSub->IsChildOf<USmithMapBaseMoveDirector>())
          {
            USmithMapBaseMoveDirector* mb_moveDirector = NewObject<USmithMapBaseMoveDirector>(obj);
            mb_moveDirector->Initialize(m_chasePlayerTracker, mapObj, moveDirector->GetChaseRadius());
            moveDirector->SetMoveDirector(mb_moveDirector);
          }
        }
      }
    }
  }

  Super::StartPlay();
}

void AMapGenerateGameMode_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void AMapGenerateGameMode_Test::StartNewStage()
{
  
}