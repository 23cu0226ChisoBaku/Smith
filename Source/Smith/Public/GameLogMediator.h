// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IOutPutLog.h"
#include "CoreMinimal.h"

class UUserWidget;
class UGameLogWidget;
/**
 *
 */
class SMITH_API GameLogMediator final : public IOutPutLog
{
public:
	GameLogMediator();
	~GameLogMediator();

public:
	void OutPutAttackLog(ISendableLog *attacker, ISendableLog *defender) override final;
	void OutPutUpgradeLog(ISendableLog *owner, ISendableLog *weapon) override final;
	void OutPutDamageLog(ISendableLog *defender, int32 damage) override final;
	void OutPutKnockDownLog(ISendableLog *downer) override final;
	void OutPutGetItemLog(ISendableLog *item, ISendableLog *rider) override final;

public:
	void SetUserWidget(UUserWidget *);

private:
	TObjectPtr<UGameLogWidget> m_widget;
};
