// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class SMITH_API UMiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// private:
	// UPROPERTY(meta = (BindWidget))

public:
	// 例えばボタン用のウィジェット配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TArray<UButton*> TileButtons;
};
