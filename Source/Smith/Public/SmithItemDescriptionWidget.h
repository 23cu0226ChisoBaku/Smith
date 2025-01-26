// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithItemDescriptionWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithItemDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USmithItemDescriptionWidget(const FObjectInitializer&);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetItemInformation(const FString& ItemName, const FString& ItemDescription);
	void ResetWidget();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionTextBlock;
};
