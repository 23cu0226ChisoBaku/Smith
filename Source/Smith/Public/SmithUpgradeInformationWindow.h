// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUpgradeInformationWindow.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
class USmithRichUpgradeParam;
struct FParams;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUpgradeInformationWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	USmithUpgradeInformationWindow(const FObjectInitializer&);

	void SetEnhancableEquipmentLabel(const FString&, int32 level);
	void SetEnhancableEquipmentImage(UTexture2D*);
	void SetParamAbsorbableMaterialLabel(const FString&);
	void SetParamAbsorbableMaterialImage(UTexture2D*);

	void SetUpgradeParams(FParams beforeParam, FParams upgradeParam);
	void SetUpgradeButtonVisibility(bool bIsVisible);

	void ResetWidget();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EnhancableEquipmentImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnhancableEquipmentLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithRichUpgradeParam> BeforeUpgradeParam;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithRichUpgradeParam> AfterUpgradeParam;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> UpgradeButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ButtonFadeEffect;


	
};
