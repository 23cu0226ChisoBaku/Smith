// Fill out your copyright notice in the Description page of Project Settings.
/*

ICanCommandMediate.h

Author : MAI ZHICONG(バクチソウ)

Description : コマンド仲介注入インタフェース

Update History: 2024/12/26 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "UObject/Interface.h"

#include "ICanCommandMediate.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ICommandMediator;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanCommandMediate : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API ICanCommandMediate
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	///
	/// @brief											コマンド仲介を注入
	/// @param	ICommandMediator*		コマンド仲介ポインタ
	///  
	virtual void SetCommandMediator(ICommandMediator*) = 0;
};
