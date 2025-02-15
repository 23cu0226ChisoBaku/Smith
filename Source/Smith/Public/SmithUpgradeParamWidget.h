// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUpgradeParamWidget.generated.h"

class UTextBlock;
struct FParams;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUpgradeParamWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USmithUpgradeParamWidget(const FObjectInitializer&);
	void UpdateParam(FParams);
	void ResetWidget();
protected:
	virtual void NativeConstruct() override;

private:
	void convertParamText(FString& outParamText, int32 param);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthPointText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackPowerText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DefensePowerText;
};
