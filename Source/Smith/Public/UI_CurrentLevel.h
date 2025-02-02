// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_CurrentLevel.generated.h"

class URichTextBlock;

/**
 *
 */
UCLASS()
class SMITH_API UUI_CurrentLevel : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override final;

public:
	void SetLevel(int32);

private:
	UPROPERTY(meta = (BindWidget))
	URichTextBlock *CurrentLevel;
};
