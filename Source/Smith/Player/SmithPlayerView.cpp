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
{ }

///
/// @brief  プレイヤーGUI周りを初期化
///
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

///
/// @brief  強化画面を表示
///
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

///
/// @brief  強化画面を非表示
///
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

///
/// @brief  強化画面の情報を初期化する
///
void USmithPlayerView::InitEnhanceMenuInfo(const FSmithWeaponInfoHandle& InfoHandle, const TArray<UObject*>& ItemObjects)
{
  if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->SetWeaponInfo(InfoHandle.Name, InfoHandle.Level, InfoHandle.Image2D);
		m_upgradeWidget->SetWeaponParam(InfoHandle.Param);
    m_upgradeWidget->SetUpgradeEntryItems(ItemObjects);
	}
}

///
/// @brief  次のアイテムを選択する
///
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

///
/// @brief  今選択しているアイテムのインデックスを取得
///
int32 USmithPlayerView::GetSelectingItemIdx() const
{
  return (m_upgradeWidget != nullptr) ? m_upgradeWidget->GetSelectingItemIdx() : -1;
}

///
/// @brief  HPを更新する
///
void USmithPlayerView::SetHP(int32 CurrentHealth, int32 MaxHealth)
{
  if (m_hpWidget != nullptr)
  {
    m_hpWidget->SetHP(CurrentHealth, MaxHealth);
  }
}

///
/// @brief  薬草の数を設定する
///
void USmithPlayerView::SetHerbValue(int32 Value)
{
  if (m_herbWidget != nullptr)
  {
    m_herbWidget->SetNum(Value);
  }
}