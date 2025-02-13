// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithRichUpgradeParam.generated.h"

class URichTextBlock;
struct FParams;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithRichUpgradeParam : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USmithRichUpgradeParam(const FObjectInitializer&);
	void UpdateParam(FParams beforeParam, FParams upgradeParam);
	void ResetWidget();

private:
	void convertParamText(FString& outParamText, int32 paramDiff) const;
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> HealthPointRichText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> AttackPowerRichText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> DefensePowerRichText;
};
