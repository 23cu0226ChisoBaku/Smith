// Fill out your copyright notice in the Description page of Project Settings.
/*

ICanSetOnMap.h

Author : MAI ZHICONG(バクチソウ)

Description : マップに配置できるオブジェクトインタフェース

Update History: 2025/01/19 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanSetOnMap.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
enum class EMapObjType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanSetOnMap : public UInterface
{
  GENERATED_BODY()
};

// TODO 再構築する予定
// パラメーターをファイルにまとめて初期化するため、パラメーターを更新するインタフェース追加
class SMITH_API ICanSetOnMap
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  ///
  /// @brief                    マップのX方向サイズを取得
  /// @return  uint8            X方向サイズ
  ///
  virtual uint8 GetOnMapSizeX() const = 0;
  ///
  /// @brief                    マップのY方向サイズを取得
  /// @return  uint8            Y方向サイズ
  ///
  virtual uint8 GetOnMapSizeY() const = 0;
  ///
  /// @brief                    マップオブジェクトの種類を取得
  /// @return  EMapObjType      種類
  ///
  virtual EMapObjType GetType() const = 0;
};
