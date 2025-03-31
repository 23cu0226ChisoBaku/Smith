// Fill out your copyright notice in the Description page of Project Settings.
/*

IEnhanceSystem.h

Author : MAI ZHICONG(バクチソウ)

Description : 強化システムインタフェース

Update History: 2025/01/23 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "UObject/Interface.h"

#include "IEnhanceSystem.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IEnhanceable;
class IParamAbsorbable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnhanceSystem : public UInterface
{
  GENERATED_BODY()
};

class SMITH_API IEnhanceSystem
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief                    強化する
  /// @param  IEnhanceable      強化できるオブジェクトインタフェース
  /// @param  IParamAbsorbable  パラメーター吸収できるオブジェクトインタフェース
  ///
  virtual void Enhance(IEnhanceable*, IParamAbsorbable*) = 0;
};
