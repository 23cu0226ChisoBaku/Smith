// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithItemInformationWindow.generated.h"

class USmithItemDescriptionWidget;
class USmithUpgradeParamWidget;
struct FParams;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithItemInformationWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDescription(const FString& name, const FString& description);
	void SetParamText(const FParams&);
	void ResetWidget();
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithItemDescriptionWidget> SmithItemDescription;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmithUpgradeParamWidget> SmithUpgradeParam;
	
};
