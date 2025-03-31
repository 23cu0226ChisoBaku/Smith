// Fill out your copyright notice in the Description page of Project Settings.
/*

IEventRegister.h

Author : MAI ZHICONG(バクチソウ)

Description : イベントレジスター

Update History: 2025/01/19

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "UObject/Interface.h"

#include "IEventRegister.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class AActor;
class ISmithMapEvent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEventRegister : public UInterface
{
  GENERATED_BODY()
};

/**
 * 
 */
class MAPMANAGEMENT_API IEventRegister
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief 								イベントを登録する
  /// @param Instigator 	  イベントを発生させたマップに配置できるオブジェクト
  /// @param ISmithMapEvent	マップイベント
  ///
  virtual void RegisterMapEvent(AActor* Instigator, ISmithMapEvent*) = 0;
};
