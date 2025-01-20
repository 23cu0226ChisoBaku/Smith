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
#include "SmithEventPublisher.h"
#include "SmithEventSystem.h"
#include "ISmithSimpleAIDriven.h"

#include "Kismet/GameplayStatics.h"

#include "SmithPlayerActor.h"
#include "SmithMapBaseMoveDirector.h"

#include "MLibrary.h"

AMapGenerateGameMode_Test::AMapGenerateGameMode_Test()
  : m_battleSystem(nullptr)
  , m_battleMediator(nullptr)
  , m_eventPublisher(nullptr)
  , m_eventSystem(nullptr)
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
  Super::StartPlay();

  initializeGame();
  startNewLevel();
}

void AMapGenerateGameMode_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void AMapGenerateGameMode_Test::startNewLevel()
{
  check(m_mapMgr.IsValid());
  check(m_battleSystem != nullptr);
  check(m_battleMediator != nullptr); 
  check(m_chasePlayerTracker != nullptr);

  APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  ICanSetOnMap* mapPlayer = Cast<ICanSetOnMap>(playerPawn);
  check(mapPlayer != nullptr);

  m_chasePlayerTracker->SetupTracker(m_mapMgr, mapPlayer);

  m_mapMgr->InitMap(GetWorld(), MapBluePrint, MapConstructionBluePrint);
  m_mapMgr->InitMapObjs(GetWorld(), playerPawn, EnemyGenerateBluePrint);
  m_mapMgr->InitMapEvents(GetWorld(), m_eventPublisher);
  m_battleSystem->InitializeBattle();

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
    
    if (moveDirectorImplementedActors.Num() > 0)
    {
      if (m_chasePlayerTracker == nullptr)
      {
        MDebug::LogError("Unexpected Error --- Init ChasePlayerTracker failed");
        return;
      }

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
  {
    TArray<AActor*> aiDrivenActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USmithSimpleAIDriven::StaticClass(), aiDrivenActors);

    if (aiDrivenActors.Num() > 0)
    {
      for (auto& aiDrivenActor : aiDrivenActors)
      {
        ISmithSimpleAIDriven* aiDriven = Cast<ISmithSimpleAIDriven>(aiDrivenActor);
        aiDriven->TurnOnAI();
      }
    }
  }
}

void AMapGenerateGameMode_Test::clearCurrentLevel()
{
  check(m_mapMgr.IsValid());
  check(m_battleSystem != nullptr);
  check(m_battleMediator != nullptr); 
  check(m_chasePlayerTracker != nullptr);

  m_mapMgr->Reset();
  m_battleSystem->ResetBattle();

}

void AMapGenerateGameMode_Test::initializeGame()
{
  m_battleSystem = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
  check(m_battleSystem != nullptr);
  
  m_battleMediator = NewObject<USmithBattleMediator>(this);
  check((m_battleMediator != nullptr));

  m_eventPublisher = NewObject<USmithEventPublisher>(this);
  check(m_eventPublisher != nullptr);

  m_eventSystem = NewObject<USmithEventSystem>(this);
  check(m_eventSystem != nullptr);

  m_chasePlayerTracker = NewObject<USmithChasePlayerTracker>(this);
  check(m_chasePlayerTracker != nullptr);
  
  m_mapMgr = ::MakeShared<UE::Smith::Map::FSmithMapManager>();
  check(m_mapMgr.IsValid());

  m_battleMediator->SetupMediator(m_battleSystem, m_mapMgr);

  m_mapMgr->AssignEventRegister(m_eventSystem);
  m_battleSystem->AssignEventExecutor(m_eventSystem);
}

void AMapGenerateGameMode_Test::goToNextLevel()
{
  clearCurrentLevel();
  startNewLevel();
}