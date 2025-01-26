// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeParamWidget.h"
#include "Components/TextBlock.h"
#include "../Weapon/Params.h"

namespace SmithUpgradeParamWidget::Private
{
  const FString HEALTH_PRE_TEXT = TEXT("HP  + ");
  const FString ATTACK_PRE_TEXT = TEXT("ATK  + ");
  const FString CRITICAL_PRE_TEXT = TEXT("CRT  + ");
}

USmithUpgradeParamWidget::USmithUpgradeParamWidget(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithUpgradeParamWidget::NativeConstruct()
{ 
  Super::NativeConstruct();
  ResetWidget();
}

void USmithUpgradeParamWidget::UpdateParam(FParams newParam)
{
  if (HealthPointTextBlock == nullptr || AttackPowerTextBlock == nullptr || CriticalTextBlock == nullptr)
  {
    return;
  }

  using namespace SmithUpgradeParamWidget::Private;
  FString healthText = HEALTH_PRE_TEXT + FString::FromInt(newParam.HP);
  FString attackText = ATTACK_PRE_TEXT + FString::FromInt(newParam.ATK);
  FString criticalText = CRITICAL_PRE_TEXT + FString::FromInt(newParam.CRT);

  HealthPointTextBlock->SetText(FText::FromString(healthText));
  AttackPowerTextBlock->SetText(FText::FromString(attackText));
  CriticalTextBlock->SetText(FText::FromString(criticalText));
}

void USmithUpgradeParamWidget::ResetWidget()
{
  if (HealthPointTextBlock != nullptr)
  {
    HealthPointTextBlock->SetText(FText{});
  }
  if (AttackPowerTextBlock != nullptr)
  {
    AttackPowerTextBlock->SetText(FText{});
  }
  if (CriticalTextBlock != nullptr)
  {
    CriticalTextBlock->SetText(FText{});
  }

  
}

