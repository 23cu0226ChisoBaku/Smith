// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogMediator.h"

#include "Debug.h"

GameLogMediator::GameLogMediator()
{

}

GameLogMediator::~GameLogMediator()
{
}

void GameLogMediator::OutPutAttackLog(ISendableLog *attacker, ISendableLog *defender)
{
  if (attacker == nullptr || defender == nullptr )
  {
    return;
  }

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = attacker->GetName() + TEXT("が") + defender->GetName() + TEXT("に攻撃");
  m_widget->AddLogMessage(message);
  m_widget->OnLog_BP();
}

void GameLogMediator::OutPutUpgradeLog(ISendableLog *owner, ISendableLog *weapon)
{
  if (owner == nullptr)
  {
    return;
  }

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = owner->GetName() + TEXT("が") + weapon->GetName() + TEXT("を強化");
  m_widget->AddLogMessage(message);
  m_widget->OnLog_BP();
}

void GameLogMediator::OutPutDamageLog(ISendableLog *defender, int32 damage)
{
  if (defender == nullptr||m_widget == nullptr)
  {
    return;
  }

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }
  FString message = defender->GetName() + TEXT("は") + FString::FromInt(damage) + TEXT("ダメージを受けた");
  m_widget->AddLogMessage(message);
  m_widget->OnLog_BP();
}

void GameLogMediator::OutPutKnockDownLog(ISendableLog *downer)
{
  if (downer == nullptr)
  {
    return;
  }

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = downer->GetName() + TEXT("は倒れた");
  m_widget->AddLogMessage(message);
  m_widget->OnLog_BP();
}

void GameLogMediator::OutPutGetItemLog(ISendableLog *item, ISendableLog *rider)
{
  if (item == nullptr || rider == nullptr)
  {
    return;
  }

  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = rider->GetName() + TEXT("は") + item->GetName() + TEXT("を手に入れた");
  m_widget->AddLogMessage(message);
  m_widget->OnLog_BP();
}


void GameLogMediator::SetUserWidget(UUserWidget* widget)
{
  m_widget = Cast<UGameLogWidget>(widget);
  if(m_widget == nullptr)
  {
    MDebug::LogError(TEXT("UGameLogWidget エラーだお"));
  }
}