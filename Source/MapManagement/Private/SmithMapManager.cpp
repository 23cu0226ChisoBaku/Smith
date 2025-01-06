// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithMapManager.h"
#include "SmithRect.h"
#include "SmithMap.h"
#include "SmithMapObj.h"

#include "ObstacleTileInfoContainer.h"
#include "StaySpaceTileInfoContainer.h"

#include "ICanSetOnMap.h"
#include "IAttackable.h"

#include "FormatTransformer.h"
#include "MapCoord.h"
#include "TileType.h"
#include "SmithCommandFormat.h"
#include "FormatType.h"
#include "MoveDirection.h"

#include "UObject/WeakInterfacePtr.h"

#include "InvalidValues.h"

#include "MLibrary.h"

#define ReturnIfTrue(expression)    \
        {                           \
          if (!!(expression))       \
          {                         \
            return;                 \
          }                         \
        }

#define ReturnIfFalse(expression)  ReturnIfTrue(!(expression))

// TODO for TMap Key adaptability
FORCEINLINE uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(ICanSetOnMap));
	return Hash;
}

namespace UE::Smith::Map::Private
{
  /// @brief アクターのユニークIDを計算する
  /// @param actor 
  /// @return ユニークID(uint32) (0 if actor is invalid)
  uint32 CalcUniqueID(AActor* actor)
  {
    if (!::IsValid(actor))
    {
      return 0;
    }

    const FString uniqueStr = actor->GetName() + FString::FromInt(actor->GetUniqueID());
    return GetTypeHash(FGuid{uniqueStr});
  }

  static const uint8 DirectionCheckBits[8] = 
  {
    0b0001,     // North
    0b0011,     // NorthEast
    0b0010,     // East
    0b0110,     // SouthEast
    0b0100,     // South
    0b1100,     // SouthWest
    0b1000,     // West
    0b1001,     // NorthWest
  };
}

namespace UE::Smith
{
  namespace Map
  {
    using namespace UE::Smith::Battle;

    class FSmithMapManager::MapMgrImpl
    {
      public:
        MapMgrImpl()
          : m_map(nullptr)
          , m_mapRect{}
          , m_onMapObjsCoordTable{}
          , m_obstacleTable{}
          , m_staySpaceTable{}
          , m_originCoord_World(FVector::ZeroVector)
          , m_mapTileSize(0)
        { }
        ~MapMgrImpl()
        {
          m_map.Reset();
          m_onMapObjsCoordTable.Empty();
          m_obstacleTable.Empty();
          m_staySpaceTable.Empty();
        }
        void AssignMap(TUniquePtr<FSmithMap>&& pMap, int32 tileSize , FVector originCoord_World)
        {
          if (!pMap.IsValid())
          {
            return;
          }

          check(tileSize > 0);

          // 例外安全？
          FSmithRect tempRect = pMap->GetMap();

          if (m_map.IsValid())
          {
            m_map.Reset();
          }

          m_map = ::MoveTemp(pMap);
          m_mapRect = ::MoveTemp(tempRect);

          m_onMapObjsCoordTable.Reset();
          m_obstacleTable.Reset();
          m_staySpaceTable.Reset();
          m_mapTileSize = tileSize;
          m_originCoord_World = originCoord_World;

          for (uint8 y = 0; y < m_mapRect.GetHeight(); ++y)
          {
            for (uint8 x = 0; x < m_mapRect.GetWidth(); ++x)
            {
              ETileType tileType = StaticCast<ETileType>(m_mapRect.GetRect(x, y));
              FMapCoord coord(x, y);
              switch (tileType)
              {
                case ETileType::Wall:
                {
                  if (!m_obstacleTable.Contains(coord))
                  {
                    m_obstacleTable.Emplace(coord, ::MakeShared<FObstacleTileInfoContainer>());
                  }
                }
                break;

                case ETileType::Ground:
                case ETileType::Corridor:
                {
                  if (!m_staySpaceTable.Contains(coord))
                  {
                    m_staySpaceTable.Emplace(coord, ::MakeShared<FStaySpaceTileInfoContainer>(tileType));
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
        }
        void AddMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
        {
          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          for (uint8 mapObjSizeX = 0; mapObjSizeX < mapObj->GetOnMapSizeX(); ++mapObjSizeX)
          {
            for (uint8 mapObjSizeY = 0; mapObjSizeY < mapObj->GetOnMapSizeY(); ++mapObjSizeY)
            {

              FMapCoord coord(x + mapObjSizeX, y + mapObjSizeY);
              if (!m_staySpaceTable.Contains(coord))
              {
                MDebug::LogError("Can not place Obj Here");
                return;
              }

              m_staySpaceTable[coord]->SetMapObj(mapObj);

            }
          }

          if (!m_onMapObjsCoordTable.Contains(mapObj))
          {
            m_onMapObjsCoordTable.Emplace(mapObj, FMapCoord{x, y});
          }

          MDebug::LogWarning("Add map obj");
        }
        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          check(m_map.IsValid());

          outActors.Reset();

          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          auto mapCoords = FFormatTransformer::FormatToMapCoord(format, format.GetCNCoord());

          for (uint64 y = 0; y < format.GetRow(); ++y)
          {
            for (uint64 x = 0; x < format.GetColumn(); ++x)
            {
              ESmithFormatType formatType = format.GetFormatData(x, y);
              if (formatType != ESmithFormatType::EFFECT)
              {
                continue;
              }

              const auto mapCoord = mapCoords.At_ReadOnly(y, x);

              if (!m_staySpaceTable.Contains(mapCoord))
              {
                continue;
              }

              ICanSetOnMap* coordMapObj = m_staySpaceTable[mapCoord]->GetMapObject();
              if (coordMapObj == nullptr || coordMapObj == mapObj)
              {
                continue;
              }

              // TODO Safe Cast may cause performance issue
              IAttackable* attackable = Cast<IAttackable>(coordMapObj);

              if (!IS_UINTERFACE_VALID(attackable))
              {
                continue;
              }

              outActors.Add(attackable);
            }
          }
        }
        void MoveMapObj(ICanSetOnMap* mapObj, UE::Smith::Battle::EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          check(m_map.IsValid());
          
          destination = InvalidValues::MapInvalidCoord;
          using namespace UE::Smith::Map::Private;

          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          if (!m_onMapObjsCoordTable.Contains(mapObj))
          {
            return;
          }

          const uint8 moveDirectionCheck_bitsIdx = StaticCast<uint8>(moveDirection);
          if (moveDirectionCheck_bitsIdx > StaticCast<uint8>(EMoveDirection::DirectionCount))
          {
            return;
          }

          if (moveDistance == 0)
          {
            return;
          }

          const uint8 directionCheck_bits = DirectionCheckBits[moveDirectionCheck_bitsIdx];
   
          uint8 realMoveDistance = 0;
          // 離れた所から探していく
          for (uint8 checkMoveDistance = 1; checkMoveDistance <= moveDistance; ++checkMoveDistance)
          {
            if (isObjectMoveable(mapObj, checkMoveDistance, directionCheck_bits))
            {
              ++realMoveDistance;
            }
          }

          updateObjCoord(mapObj, moveDirection, realMoveDistance, destination);
        }

      private:
        bool isObjectMoveable(ICanSetOnMap* mapObj, uint8 distance, uint8 checkBits_Internal) const
        {
          check(IS_UINTERFACE_VALID(mapObj));

          const FMapCoord mapObjOriginCoord = m_onMapObjsCoordTable[mapObj];

          const uint8 objSizeX = mapObj->GetOnMapSizeX();
          const uint8 objSizeY = mapObj->GetOnMapSizeY();
          // チェックが必要なマス目の最大数は横幅+縦幅を超えない
          const int32 needCheckMapObjArea = StaticCast<int32>(objSizeX) + StaticCast<int32>(objSizeY);

          int32 distanceX = 0;
          int32 distanceY = 0;
         
          TArray<FMapCoord> needCheckCoord{};
          needCheckCoord.Reserve(needCheckMapObjArea);

          uint8 bitCheck = 0b0001;//(上方向(Xのプラス方向))    
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceX += StaticCast<int32>(distance);
            for (uint8 i = 0; i < objSizeY; ++i)
            {
              needCheckCoord.Emplace(FMapCoord{StaticCast<uint8>(mapObjOriginCoord.x + objSizeX - 1), StaticCast<uint8>(mapObjOriginCoord.y + i)});
            }
          }
          bitCheck <<= 1;// 0b0010(右方向(Yのプラス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceY += StaticCast<int32>(distance);
            for (uint8 i = 0; i < objSizeX; ++i)
            {
              needCheckCoord.Emplace(FMapCoord{StaticCast<uint8>(mapObjOriginCoord.x + i), StaticCast<uint8>(mapObjOriginCoord.y + objSizeY - 1)});
            }
          }
          bitCheck <<= 1;// 0b0100(下方向(Xのマイナス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceX -= StaticCast<int32>(distance);
            for (uint8 i = 0; i < objSizeY; ++i)
            {
              needCheckCoord.Emplace(FMapCoord{mapObjOriginCoord.x, StaticCast<uint8>(mapObjOriginCoord.y + i)});
            }
          }
          bitCheck <<= 1;// 0b1000(左方向(Yのマイナス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceY -= StaticCast<int32>(distance);
            for (uint8 i = 0; i < objSizeX; ++i)
            {
              needCheckCoord.Emplace(FMapCoord{StaticCast<uint8>(mapObjOriginCoord.x + i), mapObjOriginCoord.y});
            }
          }

          // 上方向と下方向・左方向と右方向を同時にチェックしない
          check(needCheckCoord.Num() <= needCheckMapObjArea);

          for (int32 i = 0; i < needCheckCoord.Num(); ++i)
          {
            const int32 checkCoordX = StaticCast<int32>(needCheckCoord[i].x) + distanceX;
            const int32 checkCoordY = StaticCast<int32>(needCheckCoord[i].y) + distanceY;

            // 座標有効性チェック
            if (  (checkCoordX < 0) || (checkCoordX >= 256)
                ||(checkCoordY < 0) || (checkCoordY >= 256)
              )
              {
                return false;
              }
            
            const FMapCoord checkCoord(checkCoordX, checkCoordY);

            // 移動先が地面じゃない場合
            if (!m_staySpaceTable.Contains(checkCoord))
            {
              return false;
            }

            // 移動先のマスにオブジェクトが置けない
            if (!m_staySpaceTable[checkCoord]->IsAbleToStayOn())
            {
              return false;
            }
          }

          return true;
        }
        void updateObjCoord(ICanSetOnMap* mapObj, UE::Smith::Battle::EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          check(IS_UINTERFACE_VALID(mapObj));

          if (moveDistance == 0)
          {
            return;
          }

          using namespace UE::Smith::Map::Private;
          const uint8 moveDirectionCheck_bitsIdx = StaticCast<uint8>(moveDirection);
          const uint8 directionCheck_bits = DirectionCheckBits[moveDirectionCheck_bitsIdx];

          int32 distanceX = 0;
          int32 distanceY = 0;
         
          uint8 bitCheck = 0b0001;//(上方向(Xのプラス方向))    
          if ((directionCheck_bits & bitCheck) == bitCheck)
          {
            distanceX += StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b0010(右方向(Yのプラス方向))
          if ((directionCheck_bits & bitCheck) == bitCheck)
          {
            distanceY += StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b0100(下方向(Xのマイナス方向))
          if ((directionCheck_bits & bitCheck) == bitCheck)
          {
            distanceX -= StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b1000(左方向(Yのマイナス方向))
          if ((directionCheck_bits & bitCheck) == bitCheck)
          {
            distanceY -= StaticCast<int32>(moveDistance);
          }

          const FMapCoord mapObjOriginCoord = m_onMapObjsCoordTable[mapObj];
          
          // TODO ignore ground exist check;
          for (uint8 y = 0; y < mapObj->GetOnMapSizeY(); ++y)
          {
            for (uint8 x = 0; x < mapObj->GetOnMapSizeX(); ++x)
            {
              const FMapCoord removeTileCoord(mapObjOriginCoord.x + x, mapObjOriginCoord.y + y);

              const int32 fillTileCoordX = StaticCast<int32>(removeTileCoord.x) + distanceX;
              const int32 fillTileCoordY = StaticCast<int32>(removeTileCoord.y) + distanceY;
              const FMapCoord fillTileCoord(StaticCast<uint8>(fillTileCoordX), StaticCast<uint8>(fillTileCoordY));

              m_staySpaceTable[removeTileCoord]->SetMapObj(nullptr);
              m_staySpaceTable[fillTileCoord]->SetMapObj(mapObj);
            }
          }

          // TODO
          const int32 newMapObjOriginCoordX = StaticCast<int32>(mapObjOriginCoord.x) + distanceX;
          const int32 newMapObjOriginCoordY = StaticCast<int32>(mapObjOriginCoord.y) + distanceY;
          const FMapCoord newMapObjOriginCoord(StaticCast<uint8>(newMapObjOriginCoordX), StaticCast<uint8>(newMapObjOriginCoordY));
          m_onMapObjsCoordTable[mapObj] = newMapObjOriginCoord;

          // TODO
          destination = m_originCoord_World + FVector(
                                                      newMapObjOriginCoordX * m_mapTileSize, 
                                                      newMapObjOriginCoordY * m_mapTileSize, 
                                                      0.0
                                                     );
          MDebug::LogError("New X: " + FString::FromInt(newMapObjOriginCoordX));
          MDebug::LogError("New Y: " + FString::FromInt(newMapObjOriginCoordY));
        }
      private:
        TUniquePtr<FSmithMap> m_map;
        FSmithRect m_mapRect;
        TMap<TWeakInterfacePtr<ICanSetOnMap>, FMapCoord> m_onMapObjsCoordTable;
        TMap<FMapCoord, TSharedPtr<FObstacleTileInfoContainer>> m_obstacleTable;
        TMap<FMapCoord, TSharedPtr<FStaySpaceTileInfoContainer>> m_staySpaceTable;
        FVector m_originCoord_World; 
        int32 m_mapTileSize;
    };
    FSmithMapManager::FSmithMapManager()
      : m_pImpl(::MakeUnique<MapMgrImpl>())
    { }
    FSmithMapManager::~FSmithMapManager()
    {
      m_pImpl.Reset();
    }
    void FSmithMapManager::AssignMap(TUniquePtr<FSmithMap>&& pMap, int32 tileSize, FVector originCoord_World)
    {
      m_pImpl->AssignMap(::MoveTemp(pMap), tileSize, originCoord_World);
    }
    void FSmithMapManager::AddMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->AddMapObj(mapObj, x, y);
    }
    void FSmithMapManager::FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj,const FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outActors, mapObj, format);
    } 
    void FSmithMapManager::MoveMapObj(ICanSetOnMap* mapObj, UE::Smith::Battle::EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
    {
      return m_pImpl->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
    }
  }
}
