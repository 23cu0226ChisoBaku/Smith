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
  if (attacker == nullptr || defender == nullptr)
  {
    MDebug::LogError(TEXT("ISendable はエラー"));
    return;
  }

  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = attacker->GetName() + TEXT("が") + defender->GetName() + TEXT("に攻撃\n");
  m_widget->AddLogMessage(message);
  m_widget->OutPutLog();
}

void GameLogMediator::OutPutUpgradeLog(ISendableLog *owner, ISendableLog *weapon)
{
  if (owner == nullptr)
  {
    MDebug::LogError(TEXT("ISendable はエラー"));
    return;
  }

  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = owner->GetName() + TEXT("が") + weapon->GetName() + TEXT("を強化\n");
  m_widget->AddLogMessage(message);
  m_widget->OutPutLog();
}

void GameLogMediator::OutPutDamageLog(ISendableLog *defender, int32 damage)
{
  if (defender == nullptr)
  {
    MDebug::LogError(TEXT("ISendable はエラー"));
    return;
  }

  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }
  FString message = defender->GetName() + TEXT("は") + FString::FromInt(damage) + TEXT("ダメージを受けた\n");
  m_widget->AddLogMessage(message);
  m_widget->OutPutLog();
}

void GameLogMediator::OutPutKnockDownLog(ISendableLog *downer)
{
  if (downer == nullptr)
  {
    MDebug::LogError(TEXT("ISendable はエラー"));
    return;
  }

  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = downer->GetName() + TEXT("は倒れた\n");
  m_widget->AddLogMessage(message);
  m_widget->OutPutLog();
}

void GameLogMediator::OutPutGetItemLog(ISendableLog *item, ISendableLog *rider)
{
  if (item == nullptr || rider == nullptr)
  {
    MDebug::LogError(TEXT("ISendable はエラー"));
    return;
  }

  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("Widget はエラー"));
    return;
  }

  FString message = rider->GetName() + TEXT("は") + item->GetName() + TEXT("を手に入れた\n");
  m_widget->AddLogMessage(message);
  m_widget->OutPutLog();
}

void GameLogMediator::SetUserWidget(UUserWidget *widget)
{
  m_widget = Cast<UGameLogWidget>(widget);
  if (m_widget == nullptr)
  {
    MDebug::LogError(TEXT("UGameLogWidget がnull"));
  }
}