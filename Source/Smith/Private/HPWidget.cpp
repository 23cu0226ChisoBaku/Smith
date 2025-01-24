// Fill out your copyright notice in the Description page of Project Settings.


#include "HPWidget.h"
#include "Components/ProgressBar.h"
#include "Debug.h"

void UHPWidget::NativeConstruct()
{
  m_progressBar = nullptr;
}

void UHPWidget::SetHP(UUserWidget* widget,float percent)
{
  m_progressBar = Cast<UProgressBar>(widget);
  if(m_progressBar != nullptr)
  {
    m_progressBar->SetPercent(percent);
  }
  else
  {
    MDebug::LogError(TEXT("Progressbarにキャスト失敗"));
  }
}
