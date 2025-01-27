// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUpgradeWidget.generated.h"

class USmithUpgradeInformationWindow;
class USmithUIInventoryPanel;
class USmithItemInformationWindow;
class UTexture2D;

struct FSmithUpgradeItemHandle;
/**
 * 
 */
UCLASS()
class SMITH_API USmithUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWeaponInfo(const FString&, UTexture2D*);
	void SetUpgradeEntryItems(const TArray<UObject*>&);
	void SelectDown();
	void SelectUp();
	void UpdateWidgetInfo(UObject*);
	void ResetWidget();
	int32 GetSelectingItemIdx() const;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUpgradeInformationWindow> UpgradeInfoWindow;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUIInventoryPanel> InventoryPanel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithItemInformationWindow> ItemInfoWindow;

	UPROPERTY()
	TArray<UObject*> m_upgradeItems;

};
