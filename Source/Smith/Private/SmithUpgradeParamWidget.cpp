// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeParamWidget.h"
#include "Components/TextBlock.h"
#include "../Weapon/Params.h"

namespace SmithUpgradeParamWidget::Private
{
  const FString HEALTH_PRE_TEXT = TEXT("体力    ");
  const FString ATTACK_PRE_TEXT = TEXT("攻撃力  ");
  const FString CRITICAL_PRE_TEXT = TEXT("会心力  ");
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
  if (HealthPointText == nullptr || AttackPowerText== nullptr || CriticalText == nullptr)
  {
    return;
  }

  using namespace SmithUpgradeParamWidget::Private;
  FString healthText = HEALTH_PRE_TEXT;
  convertParamText(healthText, newParam.HP);
  FString attackText = ATTACK_PRE_TEXT;
  convertParamText(attackText, newParam.ATK);
  FString criticalText = CRITICAL_PRE_TEXT;
  convertParamText(criticalText, newParam.CRT);

  HealthPointText->SetText(FText::FromString(healthText));
  AttackPowerText->SetText(FText::FromString(attackText));
  CriticalText->SetText(FText::FromString(criticalText));
}

void USmithUpgradeParamWidget::ResetWidget()
{
  if (HealthPointText != nullptr)
  {
    HealthPointText->SetText(FText{});
  }
  if (AttackPowerText != nullptr)
  {
    AttackPowerText->SetText(FText{});
  }
  if (CriticalText != nullptr)
  {
    CriticalText->SetText(FText{});
  }
}

void USmithUpgradeParamWidget::convertParamText(FString& outParamText, int32 param)
{
  if (param >= 0)
  {
    outParamText.Append(TEXT("+ "));
    outParamText.Append(FString::FromInt(param));
  }
  else
  {
    outParamText.Append(TEXT("- "));
    outParamText.Append(FString::FromInt(FMath::Abs(param)));
  }
}

