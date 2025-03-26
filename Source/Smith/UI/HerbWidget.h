// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HerbWidget.generated.h"

class URichTextBlock;
/**
 *
 */
UCLASS()
class SMITH_API UHerbWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetNum(int32);

public:
	UPROPERTY(meta = (BindWidget))
	URichTextBlock *HerbNum;
};
