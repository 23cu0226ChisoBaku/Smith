// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeParamWidget.h"
#include "Components/TextBlock.h"
#include "Params.h"

namespace SmithUpgradeParamWidget::Private
{
  const FString HEALTH_PRE_TEXT = TEXT("体力    ");
  const FString ATTACK_PRE_TEXT = TEXT("攻撃力  ");
  const FString DEFENSE_PRE_TEXT = TEXT("防御力  ");
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
  if (HealthPointText == nullptr || AttackPowerText == nullptr || DefensePowerText == nullptr)
  {
    return;
  }

  using namespace SmithUpgradeParamWidget::Private;
  FString healthText = HEALTH_PRE_TEXT;
  convertParamText(healthText, newParam.HP);
  FString attackText = ATTACK_PRE_TEXT;
  convertParamText(attackText, newParam.ATK);
  FString defenseText = DEFENSE_PRE_TEXT;
  convertParamText(defenseText, newParam.DEF);

  HealthPointText->SetText(FText::FromString(healthText));
  AttackPowerText->SetText(FText::FromString(attackText));
  DefensePowerText->SetText(FText::FromString(defenseText));
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
  if (DefensePowerText != nullptr)
  {
    DefensePowerText->SetText(FText{});
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

