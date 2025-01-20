// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestMenuWidget.generated.h"

enum Menu : uint8
{
	Status,
	Item,
	Enhance
};
/**
 * 
 */
UCLASS()
class SMITH_API UTestMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	protected:
  virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	private:
	Menu m_menu;
};
