// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapConstructor.h

Author : MAI ZHICONG

Description : マップオブジェクトをワールドに配置するクラス

Update History: 2025/01/04 作成
                2025/01/10 マップタイルアクターを破棄するインターフェース追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapConstructor.h"
#include "SmithRect.h"
#include "SmithMapConstructionBluePrint.h"

#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    void FSmithMapConstructor::ConstructMap(UWorld* world, const FSmithRect& mapRect, const FSmithMapConstructionBluePrint& blueprint)
    {
      if (!::IsValid(world))
      {
        MDebug::LogError("Invalid World");
        return;
      }

      if (blueprint.TileSize <= 0)
      {
        MDebug::LogError("Invalid Tile Size");
        return;
      }

      const uint8 mapRow = mapRect.GetHeight();
      const uint8 mapColumn = mapRect.GetWidth();

      // タイルアクターのリフレクションクラスポインタを入れるバッファ
      TMap<ETileType,UClass*> tileActorUClassBuffer;
      tileActorUClassBuffer.Reserve(blueprint.TileBuildingMaterialPaths.Num());

      for (uint8 y = 0 ; y < mapRow; ++y)
      {
        for (uint8 x = 0; x < mapColumn; ++x)
        {
          const ETileType tileType = StaticCast<ETileType>(mapRect.GetRect(x,y));
          if (!blueprint.TileBuildingMaterialPaths.Contains(tileType))
          {
            MDebug::LogError("Invalid Tile Type");
            continue;
          }

          // バッファにマップ素材のActorのUClassがなかったらContentsから探して、バッファ入れておく
          if (!tileActorUClassBuffer.Contains(tileType))
          {
            // マップ素材のBPクラスを取得
            TSubclassOf<AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(*blueprint.TileBuildingMaterialPaths[tileType])).LoadSynchronous();

            if (subClass == nullptr)
            {
              MDebug::LogError("Invalid Building Material Path");
              return;
            }

            tileActorUClassBuffer.Emplace(tileType, subClass);
          }

          const FVector spawnCoord(
                                   blueprint.OriginCoordinate.X + StaticCast<double>(x * blueprint.TileSize),
                                   blueprint.OriginCoordinate.Y + StaticCast<double>(y * blueprint.TileSize),
                                   blueprint.OriginCoordinate.Z
                                  );
          
          // タイルアクターを配置
          // 壁や床タイルの向きを乱数で決めて、全体的見た目を同じく見えないようにする
          const int32 randRotator = FMath::RandRange(0,3);
          const FRotator rotation = FRotator{0.0, 90.0 * StaticCast<double>(randRotator) , 0.0};
          
          AActor* actor = world->SpawnActor<AActor>(tileActorUClassBuffer[tileType], spawnCoord, rotation);
          m_mapMaterials.Emplace(actor);
          
        }
      }
    }
    void FSmithMapConstructor::DestructMap()
    {
      for (const auto& material : m_mapMaterials)
      {
        if (material.IsValid())
        {
          material->Destroy();
        }
      }

      m_mapMaterials.Reset();
    }
  }
}
