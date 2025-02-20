// Fill out your copyright notice in the Description page of Project Settings.
/*

ICanPick.h

Author : MAI ZHICONG(バクチソウ)

Description : ものを拾えるインタフェース

Update History: 2025/01/23 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanPick.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class USmithConsumeItem;
class USmithUpgradeMaterial;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanPick : public UInterface
{
  GENERATED_BODY()
};

class SMITH_API ICanPick
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief                        消耗品アイテムを拾う
  /// @param  USmithConsumeItem     消耗品アイテム
  ///
  virtual void PickUpConsume(USmithConsumeItem*) = 0;
  ///
  /// @brief                        強化素材を拾う
  /// @param  USmithUpgradeMaterial 強化素材
  ///
  virtual void PickUpMaterial(USmithUpgradeMaterial*) = 0;
};
