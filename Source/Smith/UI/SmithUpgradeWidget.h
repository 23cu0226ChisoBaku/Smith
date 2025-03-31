// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Params.h"
#include "SmithUpgradeWidget.generated.h"

class USmithUpgradeInformationWindow;
class USmithUIInventoryPanel;
class USmithItemInformationWindow;
class USmithUpgradeMaterialPreview;
class UTexture2D;

/**
 * 
 */
UCLASS()
class SMITH_API USmithUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWeaponInfo(const FString&, int32 level, UTexture2D*);
	void SetWeaponParam(FParams);
	void SetUpgradeEntryItems(const TArray<UObject*>&);
	void SelectDown();
	void SelectUp();
	void UpdateWidgetInfo(UObject*);
	void ResetWidget();
	int32 GetSelectingItemIdx() const;

private:
	void updateUpgradeResult(FParams);

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUpgradeInformationWindow> UpgradeInfoWindow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUIInventoryPanel> InventoryPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithItemInformationWindow> ItemInfoWindow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUpgradeMaterialPreview> UpgradeMaterialPreview;

	UPROPERTY()
	TArray<UObject*> m_upgradeItems;
	
private:
	FParams m_curtWeaponParam;

};
