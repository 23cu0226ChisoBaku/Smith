// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithItemInformationWindow.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithItemInformationWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SmithItemInfomationWindow;
	
};
