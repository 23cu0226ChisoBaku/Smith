// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeWidget.h"
#include "SmithUpgradeInformationWindow.h"
#include "SmithUIInventoryPanel.h"
#include "SmithItemInformationWindow.h"
#include "ISmithItemWidgetParameterizable.h"
#include "ISmithBattleParameterizable.h"
#include "../Weapon/Params.h"

void USmithUpgradeWidget::NativeConstruct()
{
  Super::NativeConstruct();

  if (InventoryPanel != nullptr)
  {
    InventoryPanel->OnChangeItem.BindUObject(this, &USmithUpgradeWidget::UpdateWidgetInfo);
  }
}

void USmithUpgradeWidget::SetWeaponInfo(const FString& Name , UTexture2D* Image2D)
{
  if (UpgradeInfoWindow != nullptr)
  {
    UpgradeInfoWindow->SetEnhancableEquipmentLabel(Name);
    UpgradeInfoWindow->SetEnhancableEquipmentImage(Image2D);
  }
}

void USmithUpgradeWidget::SetUpgradeEntryItems(const TArray<UObject*>& entryItems)
{
  if (InventoryPanel != nullptr)
  {
    InventoryPanel->SetEntryItems(entryItems);
    m_upgradeItems = entryItems;
  }

  if (ItemInfoWindow != nullptr)
  {
    if (entryItems.Num() > 0)
    {
      UpdateWidgetInfo(entryItems[0]);
    }
  }
}

void USmithUpgradeWidget::SelectUp()
{
  if (InventoryPanel == nullptr)
  {
    return;
  }
  InventoryPanel->SelectPrevious();
}

void USmithUpgradeWidget::SelectDown()
{
  if (InventoryPanel == nullptr)
  {
    return;
  }
  InventoryPanel->SelectNext();
}

void USmithUpgradeWidget::ResetWidget()
{
  if (InventoryPanel != nullptr)
  {
    InventoryPanel->ResetWidget();
  }

  if (UpgradeInfoWindow != nullptr)
  {
    UpgradeInfoWindow->ResetWidget();
  }

  if (ItemInfoWindow != nullptr)
  {
    ItemInfoWindow->ResetWidget();
  }
}

void USmithUpgradeWidget::UpdateWidgetInfo(UObject* itemObject)
{
  if (UpgradeInfoWindow == nullptr || ItemInfoWindow == nullptr)
  {
    return;
  }

  if (itemObject == nullptr)
  {
    return;
  }

  ISmithItemWidgetParameterizable* itemParam = Cast<ISmithItemWidgetParameterizable>(itemObject);
  if (itemParam != nullptr)
  {
    UpgradeInfoWindow->SetParamAbsorbableMaterialImage(itemParam->GetIconImage());
    UpgradeInfoWindow->SetParamAbsorbableMaterialLabel(itemParam->GetName());
    ItemInfoWindow->SetDescription(itemParam->GetName(), itemParam->GetDescription());
  }

  ISmithBattleParameterizable* battleParam = Cast<ISmithBattleParameterizable>(itemObject);
  if (battleParam != nullptr)
  {
    ItemInfoWindow->SetParamText(battleParam->GetParam_Interface());
  }

}

int32 USmithUpgradeWidget::GetSelectingItemIdx() const
{
  return InventoryPanel != nullptr ? InventoryPanel->GetSelectingItemIdx() : -1;
}