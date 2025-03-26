// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUIInventoryPanel.h"
#include "Components/ListView.h"
#include "SmithUIInventoryEntry.h"
#include "MLibrary.h"

USmithUIInventoryPanel::USmithUIInventoryPanel(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_curtSelectingIdx(-1)
  , m_curtMaxItemNum(0)
{}

void USmithUIInventoryPanel::NativeConstruct()
{
  Super::NativeConstruct();
  if (InventoryListView != nullptr)
  {
    auto& onChangeEvent = InventoryListView->OnItemSelectionChanged();
    onChangeEvent.AddUObject(this, &USmithUIInventoryPanel::OnItemSelectionChangedInternal);

  }
  if (InventoryListView != nullptr)
  {
    InventoryListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
  }
}

void USmithUIInventoryPanel::SetEntryItems(const TArray<UObject*>& entryItems)
{
  if (InventoryListView != nullptr)
  {
    InventoryListView->SetListItems(entryItems);
    m_curtMaxItemNum = entryItems.Num();
    if (m_curtMaxItemNum > 0)
    {
      m_curtSelectingIdx = 0;
      InventoryListView->SetSelectedIndex(0);
      InventoryListView->ScrollIndexIntoView(0);
      InventoryListView->RequestRefresh();
    }
    else
    {
      m_curtSelectingIdx = -1;
    }
  }
}

void USmithUIInventoryPanel::ResetWidget()
{
  if (InventoryListView != nullptr)
  {
    InventoryListView->ClearListItems();
  }

  m_curtSelectingIdx = -1;
}

void USmithUIInventoryPanel::OnItemSelectionChangedInternal(UObject* itemObject)
{
  if (itemObject != InventoryListView->GetSelectedItem())
  {
    return;
  }
  OnChangeItem.ExecuteIfBound(itemObject);
}

void USmithUIInventoryPanel::SelectNext()
{
  if (InventoryListView == nullptr || m_curtMaxItemNum <= 0)
  {
    return;
  }

  m_curtSelectingIdx = (m_curtSelectingIdx + 1) % m_curtMaxItemNum;
  updateSelecting();
}

void USmithUIInventoryPanel::SelectPrevious()
{
  if (InventoryListView == nullptr || m_curtMaxItemNum <= 0)
  {
    return;
  }

  m_curtSelectingIdx = (m_curtSelectingIdx - 1) % m_curtMaxItemNum;
  if (m_curtSelectingIdx < 0)
  {
    m_curtSelectingIdx += m_curtMaxItemNum;
  }

  updateSelecting();

}

void USmithUIInventoryPanel::updateSelecting()
{
  InventoryListView->SetSelectedIndex(m_curtSelectingIdx);
  InventoryListView->ScrollIndexIntoView(m_curtSelectingIdx);
  InventoryListView->RequestRefresh();
}

int32 USmithUIInventoryPanel::GetSelectingItemIdx() const
{
  return InventoryListView != nullptr ? m_curtSelectingIdx : -1;
}

