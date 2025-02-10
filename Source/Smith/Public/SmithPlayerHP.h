// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithPlayerHP.generated.h"

class URichTextBlock;
/**
 *
 */
UCLASS()
class SMITH_API USmithPlayerHP : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHP(float);
	void SetHPNumber(int32,int32);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHP();

public:
	UPROPERTY(BluePrintReadOnly)
	float percent;
	UPROPERTY(meta = (BindWidget))
	URichTextBlock* HPNumber;
};
