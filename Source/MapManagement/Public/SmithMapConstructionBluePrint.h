// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapConstructor.h

Author : MAI ZHICONG(バクチソウ)

Description : マップに配置するオブジェクトの設計図

Update History: 2024/01/04 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "TileType.h"

#include "SmithMapConstructionBluePrint.generated.h"

/** 
 * ダンジョンマップ構築用設計図
 * ※Smith鍛冶師専用設計図
 */
USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithMapConstructionBluePrint
{
  GENERATED_BODY()

public:
  /** タイルサイズ(単位：cm) */
  UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
  int32 TileSize;

  /** マップの原点座標(マップ矩形(二次元配列)の[0,0]の座標) */
  UPROPERTY(EditAnywhere)
  FVector OriginCoordinate;
  
  /** タイルのBPオブジェクトサブクラス */
  UPROPERTY(EditAnywhere)
  TMap<ETileType, TSubclassOf<AActor>> TileBuildingMaterialSubClass;
};
