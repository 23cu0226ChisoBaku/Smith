// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithMapModelDefinition.generated.h"

UENUM()
enum class EMapModelType : uint8
{
  Player UMETA(DisplayName = "Player"),
  Enemy UMETA(DisplayName = "Enemy"),
  TypeCount UMETA(Hidden),
};	

/// @brief ダンジョンで配置するオブジェクトデータ構造体
USTRUCT()
struct FSmithMapModelData
{
  GENERATED_BODY()

  /// @brief      X方向のサイズ
  UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
  uint8 MapSizeX;

  /// @brief 			Y方向のサイズ
  UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
  uint8 MapSizeY;

  /// @brief 			オブジェクトタイプ
  UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
  EMapModelType MapModelType;

};


/// @brief 			ダンジョンに配置するオブジェクトモデル定義アセット
UCLASS(BlueprintType, Blueprintable, Const, meta = (DisplayName = "Smith Map Object Definition Asset", ShortTooltip = "Data asset to define map object data"))
class SMITHMODEL_API USmithMapModelDefinition : public UDataAsset
{
  GENERATED_BODY()

public:

  /// @brief 			具体オブジェクトのUClass（型）
  UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
  TSubclassOf<class AActor> MapObjectActorClass;

  /// @brief      データ構造体インスタンス
  UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData", meta = (DisplayName = "DataInstance"))
  FSmithMapModelData MapObjectDataInstance;
  
};
