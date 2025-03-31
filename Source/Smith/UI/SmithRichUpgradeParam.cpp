// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithRichUpgradeParam.h"
#include "Components/RichTextBlock.h"
#include "Params.h"

namespace SmithRichUpgradeParam::Private
{
  const FString HEALTH_PRE_TEXT = TEXT("体力    ");
  const FString ATTACK_PRE_TEXT = TEXT("攻撃力  ");
  const FString DEFENSE_PRE_TEXT = TEXT("防御力  ");
}

USmithRichUpgradeParam::USmithRichUpgradeParam(const FObjectInitializer& ObjectInitializer)  
  : Super(ObjectInitializer)
{}

void USmithRichUpgradeParam::NativeConstruct()
{
  Super::NativeConstruct();
  ResetWidget();
}

void USmithRichUpgradeParam::UpdateParam(FParams beforeParam, FParams upgradeParam)
{
  using namespace SmithRichUpgradeParam::Private;

  if (HealthPointRichText == nullptr || AttackPowerRichText == nullptr || DefensePowerRichText == nullptr)
  {
    return;
  }

  FString healthPointText = HEALTH_PRE_TEXT + FString::FromInt(beforeParam.HP + upgradeParam.HP);
  convertParamText(healthPointText, upgradeParam.HP);
  HealthPointRichText->SetText(FText::FromString(healthPointText));

  FString attackPowerText = ATTACK_PRE_TEXT + FString::FromInt(beforeParam.ATK + upgradeParam.ATK);
  convertParamText(attackPowerText, upgradeParam.ATK);
  AttackPowerRichText->SetText(FText::FromString(attackPowerText));

  FString defenseText = DEFENSE_PRE_TEXT + FString::FromInt(beforeParam.DEF + upgradeParam.DEF);
  convertParamText(defenseText, upgradeParam.DEF);
  DefensePowerRichText->SetText(FText::FromString(defenseText));
}

void USmithRichUpgradeParam::ResetWidget()
{
  if (HealthPointRichText != nullptr)
  {
    HealthPointRichText->SetText(FText{});
  }

  if (AttackPowerRichText != nullptr)
  {
    AttackPowerRichText->SetText(FText{});
  }

  if (DefensePowerRichText != nullptr)
  {
    DefensePowerRichText->SetText(FText{});
  }
}

void USmithRichUpgradeParam::convertParamText(FString& outParamText, int32 paramDiff) const
{
  if (paramDiff > 0)
  {
    outParamText = TEXT("<UpgradeStyle.Upgrade>") + outParamText;
  }
  else if (paramDiff < 0)
  {
    outParamText = TEXT("<UpgradeStyle.Downgrade>") + outParamText;
  }
  else
  {
    outParamText = TEXT("<UpgradeStyle.None>") + outParamText;
  }

  outParamText += TEXT("</>");
}