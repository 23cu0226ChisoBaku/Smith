// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUIInventoryPanel.generated.h"

class UListView;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUIInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	USmithUIInventoryPanel(const FObjectInitializer&);
	void SetEntryItems(const TArray<UObject*>&);
	void SelectNext();
	void SelectPrevious();
	void ResetWidget();
	int32 GetSelectingItemIdx() const;

private:
	void OnBeginVisible();
	void OnItemSelectionChangedInternal(UObject* itemObject);
	void updateSelecting();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> InventoryListView;

	int32 m_curtSelectingIdx;
	int32 m_curtMaxItemNum;

public:
	TDelegate<void(UObject*)> OnChangeItem;
	
};
