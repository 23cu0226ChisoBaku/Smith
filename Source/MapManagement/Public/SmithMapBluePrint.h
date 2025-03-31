// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapBluePrint.h

Author : MAI ZHICONG(バクチソウ)

Description : マップを作成するための設計図

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "TileType.h"

#include "SmithMapBluePrint.generated.h"

/**
 * ダンジョンマップ設計図
 * ※Smith鍛冶師専用設計図
 */
USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithMapBluePrint
{
  GENERATED_BODY()

public:
  /** セクションの行数*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 SectionRow;

  /** セクションの列数*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 SectionColumn;

  /** セクションの横幅（全てのセクションに共通する）*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 SectionWidth;

  /** セクションの縦幅（全てのセクションに共通する）*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 SectionHeight;

  /** セクションの間の隙間*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 SectionGap;

  /** 部屋の最小横幅*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomMinWidth;

  /** 部屋の最大横幅*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomMaxWidth;

  /** 部屋の最小縦幅*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomMinHeight;

  /** 部屋の最大縦幅*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomMaxHeight;

  /** 生成する部屋の最小の数*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomGenerateMinNum;

  /** 生成する部屋の最大の数*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 RoomGenerateMaxNum;

  /** セクションに埋め込むデフォルトタイル*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ETileType DefaultSectionTileType;

  /** 部屋に埋め込むデフォルトタイル*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ETileType DefaultRoomTileType;
  
  /** 通路に埋め込むデフォルトタイル*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ETileType DefaultCorridorTileType;

};
