// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleGameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"

void USmithBattleGameInstanceSubsystem::DisplayGameOverWidget(UObject* worldContextObject)
{
  UWorld* world = nullptr;
  if (::IsValid(worldContextObject))
  {
    world = worldContextObject->GetWorld();
  }
  else
  {
    if (GEngine != nullptr)
    {
      world = GEngine->GetWorld();
    }
  }

  TSubclassOf<UUserWidget> GameOverWidgetSubClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath("/Game/BP/WBP_GameOver.WBP_GameOver_C")).LoadSynchronous();
  UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(world, GameOverWidgetSubClass);

  if (GameOverWidget != nullptr)
  {
    GameOverWidget->AddToViewport();
  }

}

void USmithBattleGameInstanceSubsystem::DisplayGameClearWidget(UObject* worldContextObject)
{
  UWorld* world = nullptr;
  if (::IsValid(worldContextObject))
  {
    world = worldContextObject->GetWorld();
  }
  else
  {
    if (GEngine != nullptr)
    {
      world = GEngine->GetWorld();
    }
  }

  TSubclassOf<UUserWidget> GameClearWidgetSubClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath("/Game/BP/WBP_GameClear.WBP_GameClear_C")).LoadSynchronous();
  UUserWidget* GameClearWidget = CreateWidget<UUserWidget>(world, GameClearWidgetSubClass);

  if (GameClearWidget != nullptr)
  {
    GameClearWidget->AddToViewport();
  }
}

