// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorGameMode_Test.h"
#include "SmithBattleSubsystem.h"
#include "SmithMapManager.h"
#include "SmithBattleMediator.h"
#include "ICanCommandMediate.h"
#include "Kismet/GameplayStatics.h"

#include "TurnActor_Test.h"

ASpawnActorGameMode_Test::ASpawnActorGameMode_Test()
  : m_battleMediator(nullptr)
  , m_mapMgr(nullptr)
{
  static ConstructorHelpers::FClassFinder<APawn> PlayerBP(TEXT("/Game/BP/BP_SmithPlayerActor"));
  if (PlayerBP.Class != nullptr)
  {
    DefaultPawnClass = PlayerBP.Class;
  }

  m_mapMgr = ::MakeShared<UE::Smith::Map::FSmithMapManager>();
}

void ASpawnActorGameMode_Test::StartPlay()
{
  Super::StartPlay();

  GetWorld()->SpawnActor<ATurnActor_Test>(ATurnActor_Test::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

  // バトルシステム初期化
  USmithBattleSubsystem* subsys = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
  if (subsys == nullptr)
  {
    UE_LOG(LogTemp, Warning, TEXT("Can not init Battle System"));
    return;
  }

  m_battleMediator = NewObject<USmithBattleMediator>();
  check((m_battleMediator != nullptr));

  m_battleMediator->SetupMediator(subsys, nullptr, m_mapMgr);

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
}

void ASpawnActorGameMode_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  m_mapMgr.Reset();
}
