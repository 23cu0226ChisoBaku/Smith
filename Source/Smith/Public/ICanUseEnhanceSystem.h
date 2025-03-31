// Fill out your copyright notice in the Description page of Project Settings.
/*

ICanUseEnhanceSystem.h

Author : MAI ZHICONG(バクチソウ)

Description : 強化システムを注入するインタフェース

Update History: 2025/01/23	作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "UObject/Interface.h"

#include "ICanUseEnhanceSystem.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IEnhanceSystem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanUseEnhanceSystem : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API ICanUseEnhanceSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief                    強化システムを注入
  /// @param  IEnhanceSystem    強化システムインタフェース
  ///  
	virtual void SetEnhanceSystem(IEnhanceSystem*) = 0;
};
