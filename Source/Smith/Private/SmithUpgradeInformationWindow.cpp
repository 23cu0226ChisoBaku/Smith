// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeInformationWindow.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

USmithUpgradeInformationWindow::USmithUpgradeInformationWindow(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithUpgradeInformationWindow::NativeConstruct()
{
  Super::NativeConstruct();
  ResetWidget();
}

void USmithUpgradeInformationWindow::SetEnhancableEquipmentLabel(const FString& labelName)
{
  if (EnhancableEquipmentLabel == nullptr)
  {
    return;
  }
  EnhancableEquipmentLabel->SetText(FText::FromString(labelName));
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
  if (ParamAbsorbableMaterialLabel != nullptr)
  {
    ParamAbsorbableMaterialLabel->SetText(FText::FromString(labelName));
  }
}


void USmithUpgradeInformationWindow::SetParamAbsorbableMaterialImage(UTexture2D* texture)
{
  if (ParamAbsorbableMaterialImage == nullptr || !::IsValid(texture))
  {
    return;
  }

  ParamAbsorbableMaterialImage->SetBrushFromTexture(texture);
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

  if (ParamAbsorbableMaterialLabel != nullptr)
  {
    ParamAbsorbableMaterialLabel->SetText(FText::FromString(TEXT("強化素材を選ぶ。")));
  }

  if (ParamAbsorbableMaterialImage != nullptr)
  {
    ParamAbsorbableMaterialImage->SetBrushFromTexture(nullptr);
  }
}

