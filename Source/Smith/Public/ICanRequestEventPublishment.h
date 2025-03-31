// Fill out your copyright notice in the Description page of Project Settings.
/*

ICanRequestEventPublishment.h

Author : MAI ZHICONG(バクチソウ)

Description : イベント発行インタフェース注入インタフェース

Update History: 2025/01/23 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "UObject/Interface.h"
#include "ICanRequestEventPublishment.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IEventPublishMediator;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanRequestEventPublishment : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API ICanRequestEventPublishment
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  /// 
  /// @brief                          イベント発行仲介を注入
  /// @param  IEventPublishMediator   イベント発行仲介インタフェース
  ///
	virtual void SetEventPublishMediator(IEventPublishMediator*) = 0;
};
