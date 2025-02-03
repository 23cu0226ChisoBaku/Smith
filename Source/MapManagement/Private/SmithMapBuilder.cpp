// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapBuilder.cpp

Author : MAI ZHICONG

Description : マップを作成するクラス

Update History: 2024/12/31 作成
							: 2024/01/05 マップモデル作成関数追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapBuilder.h"
#include "SmithRect.h"
#include "SmithMap.h"
#include "SmithMapBluePrint.h"
#include "SmithMapDataModel.h"
#include "TileType.h"
#include "MapCoord.h"

// シャッフル関数(内部使用)
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

    bool FSmithMapBuilder::Build(FSmithMap* pMap, const FSmithMapBluePrint& blueprint)
    {
      if (pMap == nullptr)
      {
        return false;
      }

      using namespace SimpleArrayShuffle::Private;

      pMap->GenerateMap(blueprint.SectionRow, blueprint.SectionColumn, blueprint.SectionWidth, blueprint.SectionHeight, blueprint.SectionGap, StaticCast<uint8>(blueprint.DefaultSectionTileType));
      const uint8 sectionCnt = pMap->GetSectionCount();

      if (sectionCnt == 0)
      {
        return false;
      }

      // 作成する部屋の数を決める
      const int32 generateRandomRoomCnt = FMath::RandRange(StaticCast<int32>(blueprint.RoomGenerateMinNum), StaticCast<int32>(blueprint.RoomGenerateMaxNum));
      const int32 generateRoomCnt = (generateRandomRoomCnt > sectionCnt) ? sectionCnt : generateRandomRoomCnt;
      
      // セクションインデックスをシャッフルし、部屋を作ると作らないセクションを分ける
      TArray<uint8> sectionIdxArr{};
      sectionIdxArr.Reserve(sectionCnt);
      for (uint8 idx = 0; idx < sectionCnt; ++idx)
      {
        sectionIdxArr.Emplace(idx);
      }
      RandomShuffle(sectionIdxArr);

      // 部屋を作る
      for (int32 i = 0; i < generateRoomCnt; ++i)
      {
        generateRoom(pMap, blueprint, sectionIdxArr[i]);
      }
      // 部屋を作らない（1*1の矩形を配置）
      for (int32 i = generateRoomCnt; i < sectionCnt; ++i)
      {
        generateJoint(pMap, blueprint, sectionIdxArr[i]);
      }

      // 部屋をつないでいく
      pMap->ConnectRooms(StaticCast<uint8>(blueprint.DefaultCorridorTileType));
      return true;
    }

    TSharedPtr<FSmithMapDataModel> FSmithMapBuilder::GenerateModel(TSharedPtr<FSmithMap> pMap)
    {
      // 安全性チェック
      #pragma region Safe Check
      if (!pMap.IsValid())
      {
        return nullptr;
      }
      #pragma endregion Safe Check

      TSharedPtr<FSmithMapDataModel> model = ::MakeShared<FSmithMapDataModel>();
      const FSmithRect mapRect = pMap->GetMap();
      const uint8 mapRectWidth = mapRect.GetWidth();
      const uint8 mapRectHeight = mapRect.GetHeight();
      TMap<FMapCoord, TSharedPtr<FObstacleTileInfoContainer>> tempObstacleTable;
      TMap<FMapCoord, TSharedPtr<FStaySpaceTileInfoContainer>> tempStaySpaceTable;

      for (uint8 y = 0; y < mapRectHeight; ++y)
      {
        for (uint8 x = 0; x < mapRectWidth; ++x)
        {
          const ETileType tileType = StaticCast<ETileType>(mapRect.GetRect(x, y));
          const FMapCoord coord(x, y);
          // マップ情報を初期化する
          switch (tileType)
          {
            // 壁
            case ETileType::Wall:
            {
              if (!tempObstacleTable.Contains(coord))
              {
                tempObstacleTable.Emplace(coord, ::MakeShared<FObstacleTileInfoContainer>());
              }
            }
            break;

            // 地面
            case ETileType::Ground:
            case ETileType::Corridor:
            {
              if (!tempStaySpaceTable.Contains(coord))
              {
                tempStaySpaceTable.Emplace(coord, ::MakeShared<FStaySpaceTileInfoContainer>(tileType));
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
   
      model->Map = pMap;
      model->ObstacleTable = ::MoveTemp(tempObstacleTable);
      model->StaySpaceTable = ::MoveTemp(tempStaySpaceTable);

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
