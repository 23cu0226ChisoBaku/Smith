// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithItemInformationWindow.h"
#include "SmithItemDescriptionWidget.h"
#include "SmithUpgradeParamWidget.h"
#include "Params.h"

void USmithItemInformationWindow::NativeConstruct()
{
  Super::NativeConstruct();
}

void USmithItemInformationWindow::SetDescription(const FString& name, const FString& description)
{
  if (SmithItemDescription != nullptr)
  {
    SmithItemDescription->SetItemInformation(name, description);
  }
}

void USmithItemInformationWindow::SetParamText(const FParams& param)
{
  if (SmithUpgradeParam != nullptr)
  {
    SmithUpgradeParam->UpdateParam(param);
  }
}

void USmithItemInformationWindow::ResetWidget()
{
  if (SmithUpgradeParam != nullptr)
  {
    SmithItemDescription->ResetWidget();
    SmithUpgradeParam->ResetWidget();
  }
}

