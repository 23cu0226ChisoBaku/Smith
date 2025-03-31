// Fill out your copyright notice in the Description page of Project Settings.
/*

IEventExecutor.h

Author : MAI ZHICONG(バク チソウ)

Description : イベント実行インターフェース

Update History: 2025/01/19 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "UObject/Interface.h"

#include "IEventExecutor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEventExecutor : public UInterface
{
  GENERATED_BODY()
};

class MAPMANAGEMENT_API IEventExecutor
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief 		イベントがストックしているか
  /// @return 	イベントがストックしている		->  true
  ///						イベントがストックしていない	->  false
  ///
  virtual bool IsEventInStock() const = 0;
  ///
  ///	@brief		イベント実行
  ///
  virtual void ExecuteEvent() = 0;
};
