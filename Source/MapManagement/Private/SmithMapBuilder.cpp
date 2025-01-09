// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapBuilder.h"
#include "SmithRect.h"
#include "SmithMap.h"
#include "SmithMapBluePrint.h"
#include "SmithMapDataModel.h"
#include "TileType.h"
#include "MapCoord.h"

namespace SimpleArrayShuffle::Private
{
  template<typename T>
  void RandomShuffle(TArray<T>& arr)
  {
    const int32 arrLastIndex = arr.Num() - 1;
    for (int32 i = 0; i <= arrLastIndex; ++i)
    {
      int32 index = FMath::RandRange(i, arrLastIndex);
      if (i != index)
      {
        arr.Swap(i, index);
      }
    }
  }
}
namespace UE::Smith
{
  namespace Map
  {
    FSmithMapBuilder::FSmithMapBuilder()
    { }

    FSmithMapBuilder::~FSmithMapBuilder()
    { }

    bool FSmithMapBuilder::Build(FSmithMap* map, const FSmithMapBluePrint& blueprint)
    {
      if (map == nullptr)
      {
        return false;
      }

      using namespace SimpleArrayShuffle::Private;

      map->GenerateMap(blueprint.SectionRow, blueprint.SectionColumn, blueprint.SectionWidth, blueprint.SectionHeight, blueprint.SectionGap, StaticCast<uint8>(blueprint.DefaultSectionTileType));
      
      const uint8 sectionCnt = map->GetSectionCount();

      // 作成する部屋の数を決める
      const int32 generateRandomRoomCnt = FMath::RandRange(StaticCast<int32>(blueprint.RoomGenerateMinNum),StaticCast<int32>(blueprint.RoomGenerateMaxNum));
      const int32 generateRoomCnt = generateRandomRoomCnt > sectionCnt ? sectionCnt : generateRandomRoomCnt;
      
      TArray<uint8> sectionIdxArr{};
      sectionIdxArr.Reserve(sectionCnt);
      for (uint8 idx = 0; idx < sectionCnt; ++idx)
      {
        sectionIdxArr.Emplace(idx);
      }
      RandomShuffle(sectionIdxArr);

      for (int32 i = 0; i < generateRoomCnt; ++i)
      {
        generateRoom(map, blueprint, sectionIdxArr[i]);
      }
      for (int32 i = generateRoomCnt; i < sectionCnt; ++i)
      {
        generateJoint(map, blueprint, sectionIdxArr[i]);
      }

      map->ConnectRooms(StaticCast<uint8>(blueprint.DefaultCorridorTileType));
      return true;
    }

    TSharedPtr<FSmithMapDataModel> FSmithMapBuilder::GenerateModel(TSharedPtr<FSmithMap> pMap)
    {
      if (!pMap.IsValid())
      {
        return nullptr;
      }
      
      TSharedPtr<FSmithMapDataModel> model = ::MakeShared<FSmithMapDataModel>();

      model->Map = pMap;

      const FSmithRect mapRect = pMap->GetMap();
      const uint8 mapRectWidth = mapRect.GetWidth();
      const uint8 mapRectHeight = mapRect.GetHeight();

      for (uint8 y = 0; y < mapRectHeight; ++y)
      {
        for (uint8 x = 0; x < mapRectWidth; ++x)
        {
          ETileType tileType = StaticCast<ETileType>(mapRect.GetRect(x, y));
          FMapCoord coord(x, y);
          switch (tileType)
          {
            case ETileType::Wall:
            {
              if (!model->ObstacleTable.Contains(coord))
              {
                model->ObstacleTable.Emplace(coord, ::MakeShared<FObstacleTileInfoContainer>());
              }
            }
            break;

            case ETileType::Ground:
            case ETileType::Corridor:
            {
              if (!model->StaySpaceTable.Contains(coord))
              {
                model->StaySpaceTable.Emplace(coord, ::MakeShared<FStaySpaceTileInfoContainer>(tileType));
              }
            }
            break;
            
            case ETileType::Void:
            {

            }
            break;
          }
        }
      }

      return model;
    }

    void FSmithMapBuilder::generateRoom(FSmithMap* map, const FSmithMapBluePrint& blueprint, uint8 sectionIdx)
    {
      map->GenerateRoom(sectionIdx, blueprint.RoomMinWidth, blueprint.RoomMaxWidth, blueprint.RoomMinHeight, blueprint.RoomMaxHeight, StaticCast<uint8>(blueprint.DefaultRoomTileType));
    }
    void FSmithMapBuilder::generateJoint(FSmithMap* map, const FSmithMapBluePrint& blueprint, uint8 sectionIdx)
    {
      map->GenerateRoom(sectionIdx, 1, 1, 1, 1, StaticCast<uint8>(blueprint.DefaultRoomTileType));
    }
  }
}
