// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HPWidgetComponent.generated.h"

class UHPWidget;
/**
 *
 */
UCLASS()
class SMITH_API UHPWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UHPWidgetComponent();

public:
	void SetHP(float);

	public:
	UPROPERTY(BlueprintReadOnly)
	float Percent;
};
