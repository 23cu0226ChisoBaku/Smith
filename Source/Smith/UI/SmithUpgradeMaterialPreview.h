// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUpgradeMaterialPreview.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
/**
 * 
 */
UCLASS()
class SMITH_API USmithUpgradeMaterialPreview : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

public:
	void SetMaterialImage(UTexture2D*);
	void SetNamePlateImageColor(const FColor&);
	void SetMaterialNameLabel(const FString&);
	void ResetWidget();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MaterialImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MaterialNamePlateImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaterialNameLabel;
};
