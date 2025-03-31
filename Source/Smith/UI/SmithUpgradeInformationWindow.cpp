// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeInformationWindow.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SmithUpgradeParamWidget.h"
#include "SmithRichUpgradeParam.h"
#include "Params.h"

USmithUpgradeInformationWindow::USmithUpgradeInformationWindow(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithUpgradeInformationWindow::NativeConstruct()
{
  Super::NativeConstruct();
  ResetWidget();
}

void USmithUpgradeInformationWindow::SetEnhancableEquipmentLabel(const FString& labelName, int32 level)
{
  if (EnhancableEquipmentLabel == nullptr)
  {
    return;
  }

  FString convertedName = labelName;
  if (level > 1)
  {
    convertedName.Append(TEXT(" + "));
    convertedName.Append(FString::FromInt(level - 1));
  }
  else if (level < 1)
  {
    convertedName.Append(TEXT(" - "));
    convertedName.Append(FString::FromInt(level - 1));
  }

  EnhancableEquipmentLabel->SetText(FText::FromString(convertedName));
}

void USmithUpgradeInformationWindow::SetEnhancableEquipmentImage(UTexture2D* texture)
{
  if (EnhancableEquipmentImage == nullptr || !::IsValid(texture))
  {
    return;
  }

  EnhancableEquipmentImage->SetBrushFromTexture(texture);
}

void USmithUpgradeInformationWindow::SetParamAbsorbableMaterialLabel(const FString& labelName)
{

}


void USmithUpgradeInformationWindow::SetParamAbsorbableMaterialImage(UTexture2D* texture)
{

}

void USmithUpgradeInformationWindow::SetUpgradeParams(FParams beforeParam, FParams upgradeParam)
{
  if (BeforeUpgradeParam != nullptr)
  {
    BeforeUpgradeParam->UpdateParam(beforeParam, FParams{});
  }

  if (AfterUpgradeParam != nullptr)
  {
    AfterUpgradeParam->UpdateParam(beforeParam, upgradeParam);
  }
}

void USmithUpgradeInformationWindow::SetUpgradeButtonVisibility(bool bIsVisible)
{
  ESlateVisibility visibility;
  if (bIsVisible)
  {
    visibility = ESlateVisibility::Visible;
  }
  else
  {
    visibility = ESlateVisibility::Hidden;
  }

  if (UpgradeButton != nullptr)
  {
    UpgradeButton->SetVisibility(visibility);
  }

  if (ButtonFadeEffect != nullptr)
  {
    ButtonFadeEffect->SetVisibility(visibility);
  }
}

void USmithUpgradeInformationWindow::ResetWidget()
{
  if (EnhancableEquipmentLabel != nullptr)
  {
    EnhancableEquipmentLabel->SetText(FText{});
  }

  if (EnhancableEquipmentImage != nullptr)
  {
    EnhancableEquipmentImage->SetBrushFromTexture(nullptr);
  }

  if (BeforeUpgradeParam != nullptr)
  {
    BeforeUpgradeParam->ResetWidget();
  }

  if (AfterUpgradeParam != nullptr)
  {
    AfterUpgradeParam->ResetWidget();
  }

  SetUpgradeButtonVisibility(false);  
}

