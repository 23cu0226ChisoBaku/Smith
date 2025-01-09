// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapConstructor.h"
#include "SmithRect.h"
#include "SmithMapConstructionBluePrint.h"

#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    FSmithMapConstructor::FSmithMapConstructor()
    {
    }

    FSmithMapConstructor::~FSmithMapConstructor()
    {
    }

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

      TMap<ETileType,UClass*> tileActorBuffer;
      tileActorBuffer.Reserve(blueprint.TileBuildingMaterialPaths.Num());

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
          if (!tileActorBuffer.Contains(tileType))
          {
            // マップ素材のBPクラスを取得
            TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(*blueprint.TileBuildingMaterialPaths[tileType])).LoadSynchronous();

            if (subClass == nullptr)
            {
              MDebug::LogError("Invalid Building Material Path");
              return;
            }

            tileActorBuffer.Emplace(tileType, subClass);
          }

          const FVector spawnCoord(
                                   blueprint.OriginCoordinate.X + StaticCast<double>(x * blueprint.TileSize),
                                   blueprint.OriginCoordinate.Y + StaticCast<double>(y * blueprint.TileSize),
                                   blueprint.OriginCoordinate.Z
                                  );
          
          world->SpawnActor<AActor>(tileActorBuffer[tileType], spawnCoord, FRotator::ZeroRotator);
        }
      }
    }
  }
}
