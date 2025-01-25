// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUpgradeInformationWindow.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUpgradeInformationWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	USmithUpgradeInformationWindow(const FObjectInitializer&);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EnhancableEquipmentImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ParamAbsorbableMaterialImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnhancableEquipmentLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ParamAbsorbableMaterialLabel;


	
};
