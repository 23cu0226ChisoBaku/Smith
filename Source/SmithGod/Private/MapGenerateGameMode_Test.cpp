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
#include "ICanUseEnhanceSystem.h"
#include "TurnActor_Test.h"
#include "SmithBattleMediator.h"
#include "SmithChasePlayerTracker.h"
#include "SmithBattleSubsystem.h"
#include "SmithEnhanceSubsystem.h"
#include "SmithEventPublisher.h"
#include "SmithEventSystem.h"
#include "ISmithSimpleAIDriven.h"
#include "IEnhanceSystem.h"
#include "ICanRequestEventPublishment.h"
#include "SmithEventPublishMediator.h"
#include "SmithBattleLogWorldSubsystem.h"
#include "SmithTurnBattleWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SmithPlayerActor.h"
#include "SmithMapBaseMoveDirector.h"
#include "GameLogWidget.h"
#include "ScreenFade.h"
#include "SmithDungeonDamageCalculator.h"
#include "UI_CurrentLevel.h"
#include "SmithTowerEnemyParamInitializer.h"
#include "SmithEnemyParamInitializer.h"

#include "SmithBattlePlayerController.h"

// TODO
#include "SmithNextLevelEvent.h"
#include "SmithPickUpItemEvent.h"
#include "SmithPickable.h"
#include "NiagaraSystem.h"

#include "SmithBattleGameInstanceSubsystem.h"
#include "SmithLootGameInstanceSubsystem.h"
#include "SmithEnemyLootGenerator.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Misc/DateTime.h"
#include "AudioKit.h"
#include "ItemGenerationListRow.h"
#include "MLibrary.h"

AMapGenerateGameMode_Test::AMapGenerateGameMode_Test()
  : m_battleSystem(nullptr)
  , m_battleMediator(nullptr)
  , m_eventPublisher(nullptr)
  , m_eventSystem(nullptr)
  , m_chasePlayerTracker(nullptr)
  , m_enhanceSystem(nullptr)
  , m_eventMediator(nullptr)
  , m_logSubsystem(nullptr)
  , m_damageCalculator(nullptr)
  , m_mapMgr(nullptr)
  , m_defeatedEnemyCount(0)
  , m_curtLevel(0)
  , m_startPlayTime(0)
{ }

void AMapGenerateGameMode_Test::StartPlay()
{
  Super::StartPlay();

  initializeGame();
  startNewLevel();
}

void AMapGenerateGameMode_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  FSmithEnemyParamInitializer::DetachInitializer();
  FSmithEnemyLootGenerator::DetachLootGenerator();

  if (m_battleMediator != nullptr)
  {
    m_battleMediator->MarkAsGarbage();
  }

  if (m_eventPublisher != nullptr)
  {
    m_eventPublisher->MarkAsGarbage();
  }

  if (m_eventSystem != nullptr)
  {
    m_eventSystem->MarkAsGarbage();
  }

  if (m_chasePlayerTracker != nullptr)
  {
    m_chasePlayerTracker->MarkAsGarbage();
  }

  if (m_enhanceSystem != nullptr)
  {
    m_enhanceSystem->MarkAsGarbage();
  }

  if (m_eventMediator != nullptr)
  {
    m_eventMediator->MarkAsGarbage();
  }

  if (m_damageCalculator != nullptr)
  {
    m_damageCalculator->MarkAsGarbage();
  }

  if (m_nextLevelEvent != nullptr)
  {
    m_nextLevelEvent->ConditionalBeginDestroy();
  }

  if (m_towerInitializer != nullptr)
  {
    m_towerInitializer->MarkAsGarbage();
  }

  m_mapMgr.Reset();
}

void AMapGenerateGameMode_Test::startNewLevel()
{
  check(m_mapMgr.IsValid());
  check(m_battleSystem != nullptr);
  check(m_battleMediator != nullptr); 
  check(m_chasePlayerTracker != nullptr);
  check(m_enhanceSystem != nullptr);
  check(m_eventMediator != nullptr);

  APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  ICanSetOnMap* mapPlayer = Cast<ICanSetOnMap>(playerPawn);
  check(mapPlayer != nullptr);

  m_chasePlayerTracker->SetupTracker(m_mapMgr);

  if (m_curtLevel % 5 == 0)
  {
    m_mapMgr->InitMap(GetWorld(), BossMapBluePrint, MapConstructionBluePrint);
    m_mapMgr->InitMapObjs(GetWorld(), playerPawn, BossGenerateBluePrint);
  }
  else
  {
    m_mapMgr->InitMap(GetWorld(), MapBluePrint, MapConstructionBluePrint);
    deployNextLevelEvent();
    deployPickableEvent();
    m_mapMgr->InitMapObjs(GetWorld(), playerPawn, EnemyGenerateBluePrint);
    m_mapMgr->InitDecoration(GetWorld(), TEMP_Decoration);
  }

  m_battleSystem->InitializeBattle();

  UWorld* world = GetWorld();

  // コマンド仲介初期化
  {
    TArray<AActor*> canCmdMediateObjs;
    UGameplayStatics::GetAllActorsWithInterface(world, UCanCommandMediate::StaticClass(), canCmdMediateObjs);

    if (canCmdMediateObjs.Num() > 0)
    {
      for (auto& obj : canCmdMediateObjs)
      {
        ICanCommandMediate* mediatable = Cast<ICanCommandMediate>(obj);
        mediatable->SetCommandMediator(m_battleMediator);
      }
    }
  }

  // 敵パラメーターや倒された時のコールバック
  {
    TArray<AActor*> turnBaseEnemies;
    UGameplayStatics::GetAllActorsOfClass(world, ATurnBaseActor::StaticClass(), turnBaseEnemies);

    if (turnBaseEnemies.Num() > 0)
    {
      for (const auto& enemy : turnBaseEnemies)
      {
        ATurnBaseActor* turnBaseEnemy = Cast<ATurnBaseActor>(enemy);
        turnBaseEnemy->OnDefeatEvent.AddUObject(this, &AMapGenerateGameMode_Test::addDefeatedEnemyCount);
        if (m_curtLevel % 5 == 0)
        {
          turnBaseEnemy->OnDefeatEvent.AddUObject(this, &AMapGenerateGameMode_Test::processGameClear);
          // TODO!!!!
          ASmithPlayerActor* player = Cast<ASmithPlayerActor>(playerPawn);
          if (player != nullptr)
          {
            turnBaseEnemy->OnDefeatEvent.AddUObject(player, &ASmithPlayerActor::OnGameClear);
          }
        }
        turnBaseEnemy->InitializeParameter(m_curtLevel);
      }
    }
  }

  // 移動ストラテジー実装アクター
  {
    TArray<AActor*> moveDirectorImplementedActors;
    UGameplayStatics::GetAllActorsWithInterface(world, UMoveDirector::StaticClass(), moveDirectorImplementedActors);
    
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

        // TODO
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

  // SmithAI駆動アクター
  {
    TArray<AActor*> aiDrivenActors;
    UGameplayStatics::GetAllActorsWithInterface(world, USmithSimpleAIDriven::StaticClass(), aiDrivenActors);

    if (aiDrivenActors.Num() > 0)
    {
      for (auto& aiDrivenActor : aiDrivenActors)
      {
        ISmithSimpleAIDriven* aiDriven = Cast<ISmithSimpleAIDriven>(aiDrivenActor);
        aiDriven->TurnOnAI();
      }
    }
  }

  // イベント発行アクター
  {
    TArray<AActor*> eventPublishObjs;
    UGameplayStatics::GetAllActorsWithInterface(world, UCanRequestEventPublishment::StaticClass(), eventPublishObjs);

    if (eventPublishObjs.Num() > 0)
    {
      for (const auto& actor : eventPublishObjs)
      {
        ICanRequestEventPublishment* publishRequester = Cast<ICanRequestEventPublishment>(actor);
        publishRequester->SetEventPublishMediator(m_eventMediator);
      }
    }
  }


  m_eventSystem->Reset();

  if (CurtLevelUI != nullptr)
  {
    CurtLevelUI->SetLevel(StaticCast<int32>(m_curtLevel));
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

  // TODO
  if (m_fadeWidget != nullptr)
  {
    m_fadeWidget->StartFade(FadeStatus::In);
  }
}

void AMapGenerateGameMode_Test::initializeGame()
{
  UWorld* world = GetWorld();
  check(world != nullptr);
  ASmithTurnBattleWorldSettings* worldSettings = Cast<ASmithTurnBattleWorldSettings>(world->GetWorldSettings());

  if (worldSettings != nullptr && worldSettings->IsBattleLevel())
  {
    m_battleSystem = world->GetSubsystem<USmithBattleSubsystem>();
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

    m_damageCalculator = NewObject<USmithDungeonDamageCalculator>(this);
    check(m_damageCalculator != nullptr);
    m_damageCalculator->SetConstantNumber(TEST_DAMAGE_CALCULATOR_CONSTANT);

    m_battleMediator->SetupMediator(m_battleSystem, m_damageCalculator, m_mapMgr);

    m_mapMgr->AssignEventRegister(m_eventSystem);
    m_battleSystem->AssignEventExecutor(m_eventSystem);

    m_enhanceSystem = world->GetSubsystem<USmithEnhanceSubsystem>();
    check(m_enhanceSystem != nullptr);

    APawn* playerPawn = UGameplayStatics::GetPlayerPawn(world, 0);
    ICanUseEnhanceSystem* enhanceUser = Cast<ICanUseEnhanceSystem>(playerPawn);
    check(enhanceUser != nullptr);
    enhanceUser->SetEnhanceSystem(m_enhanceSystem);

    m_eventMediator = NewObject<USmithEventPublishMediator>(this);
    check(m_eventMediator != nullptr)

    m_eventMediator->Initialize(m_eventPublisher, m_mapMgr);

    m_logSubsystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
    check(m_logSubsystem != nullptr);

    if (LogWidgetSub != nullptr)
    {
      UGameLogWidget* logWidget = CreateWidget<UGameLogWidget>(world, LogWidgetSub);
      m_logSubsystem->SetLogWidget(logWidget);
    }

    m_curtLevel = 1;
    m_defeatedEnemyCount = 0;
    m_startPlayTime = FMath::FloorToInt32(world->GetTimeSeconds());
    m_startDateTime = FDateTime::Now();

    // TODO
    CurtLevelUI = CreateWidget<UUI_CurrentLevel>(world, LevelUISub);
    if (CurtLevelUI != nullptr)
    {
      CurtLevelUI->AddToViewport();
      CurtLevelUI->SetLevel(StaticCast<int32>(m_curtLevel));
    }

    if (FadeSub != nullptr)
    {
      m_fadeWidget = CreateWidget<UScreenFade>(world, FadeSub);

      if (m_fadeWidget != nullptr)
      {
        APlayerController* playerCtrl = world->GetFirstPlayerController();
        ASmithBattlePlayerController* smithPlayerCtrl = Cast<ASmithBattlePlayerController>(playerCtrl);
        if (smithPlayerCtrl != nullptr)
        {
          m_fadeWidget->OnFadeInStartEvent.AddUObject(this, &AMapGenerateGameMode_Test::startNewLevel);
          m_fadeWidget->OnFadeInEndEvent.AddUObject(smithPlayerCtrl, &ASmithBattlePlayerController::EnablePlayerInput);
          m_fadeWidget->OnFadeOutStartEvent.AddUObject(smithPlayerCtrl, &ASmithBattlePlayerController::DisablePlayerInput);
          m_fadeWidget->OnFadeOutEndEvent.AddUObject(this, &AMapGenerateGameMode_Test::clearCurrentLevel);
        }

        m_fadeWidget->AddToViewport(1);
        m_fadeWidget->SetVisibility(ESlateVisibility::Hidden);
      }
    }

    m_nextLevelEvent = m_eventPublisher->PublishMapEvent<USmithNextLevelEvent>(USmithNextLevelEvent::StaticClass());
    if (m_nextLevelEvent == nullptr)
    {
      MDebug::LogError("Publish failed");
    }
    else
    {
      m_nextLevelEvent->OnNextLevel.BindUObject(this, &AMapGenerateGameMode_Test::goToNextLevel);
    }

    m_towerInitializer = NewObject<USmithTowerEnemyParamInitializer>(this);
    check(m_towerInitializer != nullptr);

    m_towerInitializer->AssignEnemyParamList(EnemyDefaultParamList);
    FSmithEnemyParamInitializer::AssignInitializer(m_towerInitializer);

    UGameInstance* gameInstance = world->GetGameInstance();
    if (gameInstance != nullptr)
    {
      USmithLootGameInstanceSubsystem* lootSub = gameInstance->GetSubsystem<USmithLootGameInstanceSubsystem>();
      if (lootSub != nullptr)
      {
        lootSub->AssignLootList(EnemyDropLootList);
      }

      FSmithEnemyLootGenerator::AssignLootGenerator(lootSub);
    }
  }
}

void AMapGenerateGameMode_Test::goToNextLevel()
{
  ++m_curtLevel;
  if (m_fadeWidget != nullptr)
  {
    m_fadeWidget->StartFade(FadeStatus::Out);
  }
}

void AMapGenerateGameMode_Test::deployNextLevelEvent(bool bIsActiveWhenDeploy)
{
  if (m_mapMgr.IsValid())
  {
    m_mapMgr->InitNextLevelEvent(m_nextLevelEvent);
  }
}

void AMapGenerateGameMode_Test::addDefeatedEnemyCount()
{
  ++m_defeatedEnemyCount;
}

int32 AMapGenerateGameMode_Test::GetDefeatedEnemyCount() const
{
  return m_defeatedEnemyCount;
}

int32 AMapGenerateGameMode_Test::GetUpgradeCount() const
{
  return m_enhanceSystem != nullptr ? m_enhanceSystem->GetUpgradeCount() : 0; 
}

int32 AMapGenerateGameMode_Test::GetCurrentLevel() const
{
  return m_curtLevel;
}

int32 AMapGenerateGameMode_Test::GetCurrentPlayTime_Second() const
{
  UWorld* world = GetWorld();
  return ::IsValid(world) ? FMath::FloorToInt32(world->GetTimeSeconds()) - m_startPlayTime : 0;
}

FTimespan AMapGenerateGameMode_Test::GetCurrentPlayTime_Timespan() const
{
  return FDateTime::Now() - m_startDateTime;
}

void AMapGenerateGameMode_Test::processGameClear()
{
  UWorld* world = GetWorld();
  if (world != nullptr)
  {
    USmithBattleGameInstanceSubsystem* sub = world->GetGameInstance()->GetSubsystem<USmithBattleGameInstanceSubsystem>();
    if (sub != nullptr)
    {
      sub->DisplayGameClearWidget(world);
    }
  }
}

void AMapGenerateGameMode_Test::deployPickableEvent()
{
  if (ItemGenerationRecipe == nullptr) [[unlikely]]
  {
    return;
  }

  TArray<FItemGenerationListRow*> itemRecipes;
  ItemGenerationRecipe->GetAllRows<FItemGenerationListRow>(nullptr, itemRecipes);
  // TODO
  FString Path = TEXT("/Game/Resources/Effect/EventEffect/NS_ItemEffect.NS_ItemEffect");
  UNiagaraSystem* itemEffect = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *Path));
  for (const auto& recipe : itemRecipes)
  {
    TArray<ISmithMapEvent*> itemEvents;
    for (uint32 i = 0; i < recipe->SpawnCount; ++i)
    {
      USmithPickUpItemEvent* pickUpEvent = m_eventPublisher->PublishMapEvent<USmithPickUpItemEvent>(USmithPickUpItemEvent::StaticClass());
      if (pickUpEvent == nullptr)
      {
        break;
      }

      UObject* obj = NewObject<UObject>(this, recipe->Item);
      USmithPickable* pickable = Cast<USmithPickable>(obj);
      if (pickable == nullptr)
      {
        break;
      }

      pickUpEvent->AssignPickable(pickable, itemEffect);
      itemEvents.Emplace(pickUpEvent);
    }

    m_mapMgr->InitPickableEvent(recipe->DeployRule, itemEvents);
  }
}