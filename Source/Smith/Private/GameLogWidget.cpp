// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogWidget.h"
#include "Components/Border.h"
#include "Debug.h"

void UGameLogWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_isVisibility = false;
  m_currentAlpha = GetColorAndOpacity();
  m_borderArr.Emplace(Border1);
  m_borderArr.Emplace(Border2);
  m_borderArr.Emplace(Border3);
  m_borderArr.Emplace(Border4);

  // 全てのボーダーを非表示にする
  for (auto &arr : m_borderArr)
  {
    arr->SetVisibility((ESlateVisibility)HIDDEN);
  }
}

void UGameLogWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
  m_timer += InDeltaTime;

  if ((m_visibleTime <= m_timer) && (m_isVisibility))
  {
    // 非表示にする
    m_currentAlpha.A -= m_alphaSubtractionSpeed;
    if (m_currentAlpha.A <= 0.0f)
    {
      // リセット処理
      m_timer = 0.0f;
      m_currentAlpha.A = 0.0f;
      m_isVisibility = false;
      m_outputLogArr.Empty();

      for (auto &arr : m_borderArr)
      {
        arr->SetVisibility((ESlateVisibility)HIDDEN);
      }
      SetVisibility((ESlateVisibility)HIDDEN);
    }
    // カラー（アルファ）をセット
    SetColorAndOpacity(m_currentAlpha);
  }
}

void UGameLogWidget::AddLogMessage(FString message)
{
  m_outputLogArr.Emplace(message);
}

void UGameLogWidget::OutputLog()
{
  if (m_maxVisibleQuantity < m_outputLogArr.Num())
  {
    int32 difference = m_outputLogArr.Num() - m_maxVisibleQuantity;
    // 表示個数より超えていたら、超過した分ストレージに保存しておく
    for (int32 i = 0; i < difference; ++i)
    {
      m_logStorage.Add(m_outputLogArr[m_maxVisibleQuantity + difference - 1]);
      m_outputLogArr.RemoveAt(m_maxVisibleQuantity);
    }
  }

  // BPで出力処理
  OnLog_BP();
  
  for (int i = 0; i < m_outputLogArr.Num(); ++i)
  {
    // 配列外参照をしないようにする
    if (m_borderArr.Num() < m_outputLogArr.Num())
    {
      MDebug::LogError(TEXT("超やばい配列外参照おきる"));
      break;
    }
    m_borderArr[i]->SetVisibility((ESlateVisibility)VISIBLE);
  }

  // 表示する
  SetVisibility((ESlateVisibility)VISIBLE);
  m_currentAlpha.A = 1.0f;
  SetColorAndOpacity(m_currentAlpha);
  m_isVisibility = true;
  m_timer = 0.0f;
}

void UGameLogWidget::UpdatedLog()
{
  m_timer = 0.0f;
  m_currentAlpha.A = 1.0f;
  SetColorAndOpacity(m_currentAlpha);
}