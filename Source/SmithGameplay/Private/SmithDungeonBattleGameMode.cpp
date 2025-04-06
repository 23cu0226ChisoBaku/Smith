// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDungeonBattleGameMode.h"

#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithMapSystem.h"
#include "ICanCommandMediate.h"
#include "IMoveDirector.h"
#include "ICanUseEnhanceSystem.h"
#include "SmithCommandMediator.h"
#include "SmithChasePlayerTracker.h"
#include "SmithBattleSubsystem.h"
#include "SmithDamageSubsystem.h"
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
#include "UI_CurrentLevel.h"
#include "SmithTowerEnemyParamInitializer.h"
#include "SmithEnemyParamInitializer.h"
#include "SmithPlayerController.h"
#include "SmithMinimap.h"
#include "MinimapDisplayTypeFactory.h"
#include "SmithNextLevelEvent.h"
#include "SmithPickUpItemEvent.h"
#include "NiagaraSystem.h"
#include "SmithBattleGameInstanceSubsystem.h"
#include "SmithLootGameInstanceSubsystem.h"
#include "SmithEnemyLootGenerator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/DateTime.h"
#include "ItemGenerationListRow.h"
#include "DamageCalculationStrategies.h"
#include "TurnBaseActor.h"

#include "SmithMapModelRepository.h"
#include "SmithBattleLogModelRepository.h"
#include "SmithEventModelRepository.h"

#include "MapModelMapper.h"
#include "EventModelMapper.h"
#include "BattleLogModelMapper.h"

#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithDungeonBattleGameMode)

ASmithDungeonBattleGameMode::ASmithDungeonBattleGameMode()
  : m_battleSystem(nullptr)
  , m_battleMediator(nullptr)
  , m_eventPublisher(nullptr)
  , m_eventSystem(nullptr)
  , m_chasePlayerTracker(nullptr)
  , m_enhanceSystem(nullptr)
  , m_eventMediator(nullptr)
  , m_logSubsystem(nullptr)
  , m_mapSys(nullptr)
  , m_defeatedEnemyCount(0)
  , m_curtLevel(0)
  , m_startPlayTime(0)
{ }

void ASmithDungeonBattleGameMode::StartPlay()
{
  Super::StartPlay();

  initializeGame();
  startNewLevel();
}

void ASmithDungeonBattleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  USmithEnemyParamInitializer::DetachInitializer();
  USmithEnemyLootGenerator::DetachLootGenerator();

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

  if (m_nextLevelEvent != nullptr)
  {
    m_nextLevelEvent->ConditionalBeginDestroy();
  }


  m_mapSys.Reset();
}

void ASmithDungeonBattleGameMode::startNewLevel()
{
  APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

  // ボス階層初期化
  if (m_curtLevel % 5 == 0)
  {
    m_mapSys->InitMap(GetWorld(), BossMapBluePrint, MapConstructionBluePrint);
    m_mapSys->InitMapObjs(GetWorld(), playerPawn, BossGenerateBluePrint);
  }
  // 普通階層初期化
  else
  {
    m_mapSys->InitMap(GetWorld(), MapBluePrint, MapConstructionBluePrint);
    deployNextLevelEvent();
    deployPickableEvent();
    m_mapSys->InitMapObjs(GetWorld(), playerPawn, EnemyGenerateBluePrint);
    m_mapSys->InitDecoration(GetWorld(), TEMP_Decoration);
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
        turnBaseEnemy->OnDefeatEvent.AddUObject(this, &ThisClass::addDefeatedEnemyCount);
        // TODO 
        if (m_curtLevel % 5 == 0)
        {
          turnBaseEnemy->OnDefeatEvent.AddUObject(this, &ThisClass::processGameClear);
          // TODO!!!!
          ASmithPlayerActor* playerModel = Cast<ASmithPlayerActor>(playerPawn);
          if (playerModel != nullptr)
          {
            turnBaseEnemy->OnDefeatEvent.AddUObject(playerModel, &ASmithPlayerActor::OnGameClear);
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

      for (AActor* moveDirectorActor : moveDirectorImplementedActors)
      {
        IMoveDirector* moveDirector = Cast<IMoveDirector>(moveDirectorActor);
        UClass* moveDirectorSub = moveDirector->GetMoveDirectorUClass();

        if (moveDirectorSub->IsChildOf<USmithMapBaseMoveDirector>())
        {
          USmithMapBaseMoveDirector* mb_moveDirector = NewObject<USmithMapBaseMoveDirector>(moveDirectorActor);
          mb_moveDirector->Initialize(m_chasePlayerTracker, moveDirectorActor, moveDirector->GetChaseRadius());
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

void ASmithDungeonBattleGameMode::clearCurrentLevel()
{
  m_mapSys->Reset();
  m_battleSystem->ResetBattle();
  m_minimap->ResetMap();

  if (m_fadeWidget != nullptr)
  {
    m_fadeWidget->StartFade(FadeStatus::In);
  }
}

void ASmithDungeonBattleGameMode::initializeGame()
{ 
  UWorld* world = GetWorld();
  check(world != nullptr);

  ASmithTurnBattleWorldSettings* worldSettings = Cast<ASmithTurnBattleWorldSettings>(world->GetWorldSettings());
  if (worldSettings != nullptr && worldSettings->IsBattleLevel())
  {
    m_battleSystem = world->GetSubsystem<USmithBattleSubsystem>();
    check(m_battleSystem != nullptr);
    
    m_battleMediator = NewObject<USmithCommandMediator>(this);
    check((m_battleMediator != nullptr));

    m_eventPublisher = NewObject<USmithEventPublisher>(this);
    check(m_eventPublisher != nullptr);

    m_eventSystem = NewObject<USmithEventSystem>(this);
    check(m_eventSystem != nullptr);

    m_chasePlayerTracker = NewObject<USmithChasePlayerTracker>(this);
    check(m_chasePlayerTracker != nullptr);
    
    m_mapSys = ::MakeShared<UE::Smith::Map::FSmithMapSystem>();
    check(m_mapSys.IsValid());

    m_chasePlayerTracker->SetupTracker(m_mapSys);

    USmithDamageSubsystem* damageSys = world->GetSubsystem<USmithDamageSubsystem>();
    if (damageSys != nullptr)
    {
      damageSys->SetDamageStrategy(FSmithReductionRateDCS{TEST_DAMAGE_CALCULATOR_CONSTANT});
    }
    // コマンド仲介初期化
    m_battleMediator->SetupMediator(m_battleSystem, damageSys, m_mapSys);

    // イベント登録システムを注入
    m_mapSys->AssignEventRegister(m_eventSystem);
    m_battleSystem->AssignEventExecutor(m_eventSystem);

    {
      USmithMapModelRepository* mapModelRepository = world->GetSubsystem<USmithMapModelRepository>();
      if (mapModelRepository != nullptr)
      {
        mapModelRepository->InitializeMapModel(MapModelMapper);
        m_mapSys->AssignMapModelRequester(mapModelRepository);
      }
    }

    m_enhanceSystem = world->GetSubsystem<USmithEnhanceSubsystem>();
    check(m_enhanceSystem != nullptr);

    APawn* playerPawn = UGameplayStatics::GetPlayerPawn(world, 0);
    // 強化システムを注入
    ICanUseEnhanceSystem* enhanceUser = Cast<ICanUseEnhanceSystem>(playerPawn);
    check(enhanceUser != nullptr);
    enhanceUser->SetEnhanceSystem(m_enhanceSystem);

    ASmithPlayerActor* player = Cast<ASmithPlayerActor>(playerPawn);
    if (player != nullptr)
    {
      player->OnDead.AddUObject(this, &ThisClass::processGameOver);
    }

    m_eventMediator = NewObject<USmithEventPublishMediator>(this);
    check(m_eventMediator != nullptr)

    m_eventMediator->Initialize(m_eventPublisher, m_mapSys);

    {
      m_logSubsystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
      check(m_logSubsystem != nullptr);
  
      if (LogWidgetSub != nullptr)
      {
        UGameLogWidget* logWidget = CreateWidget<UGameLogWidget>(world, LogWidgetSub);
        m_logSubsystem->SetLogWidget(logWidget);
      }

      USmithBattleLogModelRepository* logModelRepository = world->GetSubsystem<USmithBattleLogModelRepository>();
      if (logModelRepository != nullptr)
      {
        logModelRepository->InitializeBattleLogModel(LogModelMapper);
      }

      USmithEventModelRepository* eventModelRepository = world->GetSubsystem<USmithEventModelRepository>();
      if (eventModelRepository != nullptr)
      {
        eventModelRepository->InitializeEventModel(EventModelMapper);
        m_logSubsystem->AssignLogRepository(logModelRepository, eventModelRepository);

      }
    }
    
    // リザルト変数初期化
    m_curtLevel = 1;
    m_defeatedEnemyCount = 0;
    m_startPlayTime = FMath::FloorToInt32(world->GetTimeSeconds());
    m_startDateTime = FDateTime::Now();

    // 階層表示ウイジェット
    CurtLevelUI = CreateWidget<UUI_CurrentLevel>(world, LevelUISub);
    if (CurtLevelUI != nullptr)
    {
      CurtLevelUI->AddToViewport();
      CurtLevelUI->SetLevel(StaticCast<int32>(m_curtLevel));
    }

    // Minimap
    {
      m_minimap = CreateWidget<USmithMinimap>(world, MinimapWidgetSub);
      check(m_minimap != nullptr)
      if (m_minimap != nullptr)
      {
        m_minimap->AddToViewport();
      }
  
      m_factory = NewObject<UMinimapDisplayTypeFactory>(world, MinimapTypeFactorySub);
      check(m_factory != nullptr);
      
      m_mapSys->AssignMinimapDisplayer(m_minimap);
      m_mapSys->AssignMinimapDisplayTypeFactory(m_factory);
    }

    if (FadeSub != nullptr)
    {
      m_fadeWidget = CreateWidget<UScreenFade>(world, FadeSub);

      if (m_fadeWidget != nullptr)
      {
        APlayerController* playerCtrl = world->GetFirstPlayerController();
        ASmithPlayerController* smithPlayerCtrl = Cast<ASmithPlayerController>(playerCtrl);
        if (smithPlayerCtrl != nullptr)
        {
          m_fadeWidget->OnFadeInStartEvent.AddUObject(this, &ThisClass::startNewLevel);
          m_fadeWidget->OnFadeInEndEvent.AddUObject(smithPlayerCtrl, &ASmithPlayerController::EnablePlayerInput);
          m_fadeWidget->OnFadeOutStartEvent.AddUObject(smithPlayerCtrl, &ASmithPlayerController::DisablePlayerInput);
          // TODO
          m_fadeWidget->OnFadeOutStartEvent.AddUObject(m_battleSystem, &USmithBattleSubsystem::ResetBattle);
          m_fadeWidget->OnFadeOutEndEvent.AddUObject(this, &ThisClass::clearCurrentLevel);

          smithPlayerCtrl->OnEnhanceMenuOpened.AddUObject(this, &ThisClass::hideMinimap);
          smithPlayerCtrl->OnEnhanceMenuClosed.AddUObject(this, &ThisClass::showMinimap);
        }

        m_fadeWidget->AddToViewport(1);
        m_fadeWidget->SetVisibility(ESlateVisibility::Hidden);
      }
    }

    // 階層上りイベント
    m_nextLevelEvent = m_eventPublisher->PublishMapEvent<USmithNextLevelEvent>(USmithNextLevelEvent::StaticClass());
    if (m_nextLevelEvent == nullptr)
    {
      MDebug::LogError("Publish failed");
    }
    else
    {
      m_nextLevelEvent->OnNextLevel.BindUObject(this, &ThisClass::goToNextLevel);
    }

    // TODO DLLをロードすると、スタティック変数のコピーが生成される
    // 故に、違う値に代入してしまう
    // 解決策：ヘルパーDLLを用意して、他のDLLがそのDLLをアクセスすると値の一致性が保障される？
    const UGameInstance* gameInstance = world->GetGameInstance();
    USmithLootGameInstanceSubsystem* lootGenerator = gameInstance->GetSubsystem<USmithLootGameInstanceSubsystem>();
    if (lootGenerator != nullptr)
    {
      lootGenerator->AssignLootList(EnemyDropLootList);
      USmithEnemyLootGenerator::AssignLootGenerator(lootGenerator);
    }

    USmithTowerEnemyParamInitializer* paramInitializer = gameInstance->GetSubsystem<USmithTowerEnemyParamInitializer>();
    if (paramInitializer != nullptr)
    {
      paramInitializer->AssignEnemyParamList(EnemyDefaultParamList);
      USmithEnemyParamInitializer::AssignInitializer(paramInitializer);
    }
    
  }
}

void ASmithDungeonBattleGameMode::goToNextLevel()
{
  ++m_curtLevel;
  if (m_fadeWidget != nullptr)
  {
    m_fadeWidget->StartFade(FadeStatus::Out);
  }
}

void ASmithDungeonBattleGameMode::deployNextLevelEvent(bool bIsActiveWhenDeploy)
{
  if (m_mapSys.IsValid())
  {
    m_mapSys->InitNextLevelEvent(m_nextLevelEvent);
  }
}

void ASmithDungeonBattleGameMode::addDefeatedEnemyCount()
{
  ++m_defeatedEnemyCount;
}

int32 ASmithDungeonBattleGameMode::GetDefeatedEnemyCount() const
{
  return m_defeatedEnemyCount;
}

int32 ASmithDungeonBattleGameMode::GetUpgradeCount() const
{
  return m_enhanceSystem != nullptr ? m_enhanceSystem->GetUpgradeCount() : 0; 
}

int32 ASmithDungeonBattleGameMode::GetCurrentLevel() const
{
  return m_curtLevel;
}

int32 ASmithDungeonBattleGameMode::GetCurrentPlayTime_Second() const
{
  const UWorld* world = GetWorld();
  return ::IsValid(world) ? FMath::FloorToInt32(world->GetTimeSeconds()) - m_startPlayTime : 0;
}

FTimespan ASmithDungeonBattleGameMode::GetCurrentPlayTime_Timespan() const
{
  return FDateTime::Now() - m_startDateTime;
}

void ASmithDungeonBattleGameMode::processGameClear()
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

void ASmithDungeonBattleGameMode::deployPickableEvent()
{
  if (ItemGenerationRecipe == nullptr)
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
      IPickable* pickable = Cast<IPickable>(obj);
      if (pickable == nullptr)
      {
        break;
      }

      pickUpEvent->AssignPickable(pickable, itemEffect);
      itemEvents.Emplace(pickUpEvent);
    }

    m_mapSys->InitPickableEvent(recipe->DeployRule, itemEvents);
  }
}

void ASmithDungeonBattleGameMode::processGameOver()
{
  const UWorld* world = GetWorld();
  if (world != nullptr)
  {
    UGameInstance* gameInstance = world->GetGameInstance();
    USmithBattleGameInstanceSubsystem* battleInstanceSubsystem = gameInstance->GetSubsystem<USmithBattleGameInstanceSubsystem>();
    if (battleInstanceSubsystem != nullptr)
    {
      battleInstanceSubsystem->DisplayGameOverWidget(this);
    }
  }
}

void ASmithDungeonBattleGameMode::hideMinimap()
{
  if (m_minimap != nullptr)
  {
    m_minimap->SetVisibility(false);
  }
}

void ASmithDungeonBattleGameMode::showMinimap()
{
  if (m_minimap != nullptr)
  {
    m_minimap->SetVisibility(true);
  }
}