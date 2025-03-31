// Fill out your copyright notice in the Description page of Project Settings.
/*

ISmithMapEvent.h

Author : MAI ZHICONG

Description : マップイベントクラス

Update History: 2025/01/19 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#include "UObject/Interface.h"

#include "ISmithMapEvent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithMapEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
// TODO インターフェースを抽象クラスにする
class SMITH_API ISmithMapEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitializeEvent(const FVector&, const FRotator& = FRotator::ZeroRotator) = 0;
	virtual void TriggerEvent(AActor* Instigator) = 0;
	virtual void DiscardEvent() = 0;
	virtual void RaiseEvent() = 0;
	virtual bool IsDisposed() const = 0;
	virtual void ConsumeEvent() {};

};
