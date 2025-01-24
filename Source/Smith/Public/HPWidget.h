// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPWidget.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class SMITH_API UHPWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetHP(UUserWidget*,float);

private:
	UProgressBar* m_progressBar;
};
