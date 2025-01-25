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
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthPointTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackPowerTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CriticalTextBlock;
};
