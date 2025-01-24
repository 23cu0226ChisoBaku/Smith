// Fill out your copyright notice in the Description page of Project Settings.


#include "HPWidgetComponent.h"
#include "HPWidget.h"
#include "Debug.h"

UHPWidgetComponent::UHPWidgetComponent()
{
  m_widget = NewObject<UHPWidget>();

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("HPWidgetが生成できない"));
  }
}

void UHPWidgetComponent::SetHP(float percent)
{
  UUserWidget* widget = Cast<UUserWidget>(m_widget);
  if(widget != nullptr)
  {
    m_widget->SetHP(widget,percent);
  }
  else
  {
    MDebug::LogError(TEXT("UserWidgetにキャスト失敗"));
  }
}