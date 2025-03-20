// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPlayerView.h"

#include "SmithUpgradeWidget.h"
#include "SmithPlayerHP.h"
#include "HerbWidget.h"

#include "GameFramework/PlayerController.h"
#include "SmithWeapon.h"

#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithPlayerView)

USmithPlayerView::USmithPlayerView(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_upgradeWidget(nullptr)
  , m_hpWidget(nullptr)
  , m_herbWidget(nullptr)
{
}

void USmithPlayerView::InitPlayerView(APlayerController* PlayerController)
{
  check(PlayerController != nullptr);
  check(UpgradeWidgetClass != nullptr);
  check(HPWidgetClass != nullptr);
  check(HerbWidgetClass != nullptr);

  m_hpWidget = CreateWidget<USmithPlayerHP>(PlayerController, HPWidgetClass);
  m_herbWidget = CreateWidget<UHerbWidget>(PlayerController, HerbWidgetClass);

  m_upgradeWidget = CreateWidget<USmithUpgradeWidget>(PlayerController, UpgradeWidgetClass);

  m_hpWidget->AddToViewport();
  m_herbWidget->AddToViewport();

  m_upgradeWidget->AddToViewport();
  HideUpgradeMenu();
}

void USmithPlayerView::ShowUpgradeMenu()
{
  if (m_hpWidget != nullptr)
  {
    m_hpWidget->SetVisibility(ESlateVisibility::Hidden);
  }

  if (m_herbWidget != nullptr)
  {
    m_herbWidget->SetVisibility(ESlateVisibility::Hidden);
  }

  if (m_upgradeWidget != nullptr)
  {
    m_upgradeWidget->SetVisibility(ESlateVisibility::Visible);
  }
}

void USmithPlayerView::HideUpgradeMenu()
{
  if (m_hpWidget != nullptr)
  {
    m_hpWidget->SetVisibility(ESlateVisibility::Visible);
  }

  if (m_herbWidget != nullptr)
  {
    m_herbWidget->SetVisibility(ESlateVisibility::Visible);
  }

  if (m_upgradeWidget != nullptr)
  {
    m_upgradeWidget->ResetWidget();
    m_upgradeWidget->SetVisibility(ESlateVisibility::Hidden);
  }
}

void USmithPlayerView::InitEnhanceMenuInfo(const FSmithWeaponInfoHandle& InfoHandle, const TArray<UObject*>& ItemObjects)
{
  if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->SetWeaponInfo(InfoHandle.Name, InfoHandle.Level, InfoHandle.Image2D);
		m_upgradeWidget->SetWeaponParam(InfoHandle.Param);
    m_upgradeWidget->SetUpgradeEntryItems(ItemObjects);
	}
}

void USmithPlayerView::SelectNextMenuItem(float SelectDirection)
{
  using namespace MLibrary::UE::Audio;

  if ((m_upgradeWidget == nullptr) || (!m_upgradeWidget->IsVisible()))
	{
    return;
  }

  if (SelectDirection > 0)
  {
    m_upgradeWidget->SelectDown();
    AudioKit::PlaySE(TEXT("Player_Select_2"));
  }
  else if (SelectDirection < 0)
  {
    m_upgradeWidget->SelectUp();
    AudioKit::PlaySE(TEXT("Player_Select_1"));
  }
}


int32 USmithPlayerView::GetSelectingItemIdx() const
{
  return (m_upgradeWidget != nullptr) ? m_upgradeWidget->GetSelectingItemIdx() : -1;
}

void USmithPlayerView::SetHP(int32 CurrentHealth, int32 MaxHealth)
{
  if (m_hpWidget != nullptr)
  {
    m_hpWidget->SetHP(CurrentHealth, MaxHealth);
  }
}

void USmithPlayerView::SetHerbValue(int32 Value)
{
  if (m_herbWidget != nullptr)
  {
    m_herbWidget->SetNum(Value);
  }
}