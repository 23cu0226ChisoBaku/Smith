// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithPlayerHP.generated.h"

/**
 *
 */
UCLASS()
class SMITH_API USmithPlayerHP : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHP(float);
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHP();

public:
	UPROPERTY(BluePrintReadOnly)
	float percent;
};
