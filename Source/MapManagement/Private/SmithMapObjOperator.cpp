// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjOperator.h

Author : MAI ZHICONG

Description : マップオブジェクトを操作する(移動など)クラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#include "SmithMapObjOperator.h"
#include "SmithMap.h"
#include "SmithMapDataModel.h"
#include "InvalidValues.h"
#include "ICanSetOnMap.h"
#include "MoveDirection.h"
#include "SmithCommandFormat.h"
#include "FormatType.h"
#include "FormatTransformer.h"
#include "IAttackable.h"

#include "MLibrary.h"

// 内部使用
namespace UE::Smith
{
  namespace Map
  {
    namespace Private
    {
      // 移動先チェックビット
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
    using namespace UE::Smith::Battle;
    
    ///
    /// @brief FSmithMapObjOperator実装クラス
    ///
    class FSmithMapObjOperator::MapObjOperatorImpl
    {
      using Model = typename FSmithMapDataModel;
      public:
        MapObjOperatorImpl()
          : m_model(nullptr)
          , m_originCoord_World(FVector::ZeroVector)
          , m_mapTileSize(0)
        { }
        ~MapObjOperatorImpl()
        { }

        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          check(m_model.IsValid());
          if (!m_model.IsValid())
          {
            return;
          }

          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          outActors.Reset();

          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            return;
          }

          // オブジェクトの中心座標
          const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj];
          UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord> mapCoords = FFormatTransformer::FormatToMapCoord(format, mapObjOriginCoord);

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

              if (!model_shared->StaySpaceTable.Contains(mapCoord))
              {
                continue;
              }

              MDebug::LogWarning("Attacking X:" + FString::FromInt(mapCoord.x) + "Y:" + FString::FromInt(mapCoord.y));
              ICanSetOnMap* coordMapObj = model_shared->StaySpaceTable[mapCoord]->GetMapObject();
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
        void MoveMapObj(ICanSetOnMap* mapObj, EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          if (!m_model.IsValid())
          {
            return;
          }

          if (!IS_UINTERFACE_VALID(mapObj) || moveDistance == 0)
          {
            return;
          }

          const uint8 moveDirectionCheck_bitsIdx = StaticCast<uint8>(moveDirection);
          if (moveDirectionCheck_bitsIdx > StaticCast<uint8>(EMoveDirection::DirectionCount))
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();

          if (model_shared.IsValid())
          {
            using namespace Private;
            destination = InvalidValues::MapInvalidCoord;

            if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
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

            updateObjCoord(mapObj, directionCheck_bits, realMoveDistance, destination);
          }
          
        }
        void AssignMap(TSharedPtr<Model> model, FVector originCoord_World, int32 tileSize)
        {
          m_model = model;
          m_originCoord_World = originCoord_World;
          m_mapTileSize = tileSize;
        }

      private:
        bool isObjectMoveable(ICanSetOnMap* mapObj, uint8 distance, uint8 checkBits_Internal) const
        {
          check(IS_UINTERFACE_VALID(mapObj));
          check(m_model.IsValid());

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return false;
          }

          const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj];

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
            // かつ
            // 移動先のマスにオブジェクトが置けない
            if (   !model_shared->StaySpaceTable.Contains(checkCoord) 
                || !model_shared->StaySpaceTable[checkCoord]->IsAbleToStayOn()
               )
            {
              return false;
            }
          }

          return true;
        }
        void updateObjCoord(ICanSetOnMap* mapObj, uint8 checkBits_Internal, uint8 moveDistance, FVector& destination)
        {
          check(IS_UINTERFACE_VALID(mapObj));
          check(m_model.IsValid());
          check(m_mapTileSize > 0);

          if (moveDistance == 0)
          {
            return;
          }

          TSharedPtr<FSmithMapDataModel> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          int32 distanceX = 0;
          int32 distanceY = 0;
         
          uint8 bitCheck = 0b0001;//(上方向(Xのプラス方向))    
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceX += StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b0010(右方向(Yのプラス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceY += StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b0100(下方向(Xのマイナス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceX -= StaticCast<int32>(moveDistance);
          }
          bitCheck <<= 1;// 0b1000(左方向(Yのマイナス方向))
          if ((checkBits_Internal & bitCheck) == bitCheck)
          {
            distanceY -= StaticCast<int32>(moveDistance);
          }

          if (distanceX == 0 && distanceY ==0)
          {
            return;
          }

          const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj];
          
          // TODO ignore ground exist check;
          for (uint8 y = 0; y < mapObj->GetOnMapSizeY(); ++y)
          {
            for (uint8 x = 0; x < mapObj->GetOnMapSizeX(); ++x)
            {
              const FMapCoord removeTileCoord(mapObjOriginCoord.x + x, mapObjOriginCoord.y + y);

              const int32 fillTileCoordX = StaticCast<int32>(removeTileCoord.x) + distanceX;
              const int32 fillTileCoordY = StaticCast<int32>(removeTileCoord.y) + distanceY;
              const FMapCoord fillTileCoord(StaticCast<uint8>(fillTileCoordX), StaticCast<uint8>(fillTileCoordY));

              model_shared->StaySpaceTable[removeTileCoord]->SetMapObj(nullptr);
              model_shared->StaySpaceTable[fillTileCoord]->SetMapObj(mapObj);
            }
          }

          // TODO
          const int32 newMapObjOriginCoordX = StaticCast<int32>(mapObjOriginCoord.x) + distanceX;
          const int32 newMapObjOriginCoordY = StaticCast<int32>(mapObjOriginCoord.y) + distanceY;
          const FMapCoord newMapObjOriginCoord(StaticCast<uint8>(newMapObjOriginCoordX), StaticCast<uint8>(newMapObjOriginCoordY));
          model_shared->OnMapObjsCoordTable[mapObj] = newMapObjOriginCoord;

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
        TWeakPtr<Model> m_model;
        FVector m_originCoord_World;
        int32 m_mapTileSize;
    };

    FSmithMapObjOperator::FSmithMapObjOperator()
      : m_pImpl(::MakeUnique<MapObjOperatorImpl>())
    {
    }

    FSmithMapObjOperator::~FSmithMapObjOperator()
    {
      m_pImpl.Reset();
    }

    FSmithMapObjOperator::FSmithMapObjOperator(FSmithMapObjOperator&& other) noexcept
      : m_pImpl(::MoveTemp(other.m_pImpl))
    { }

    FSmithMapObjOperator& FSmithMapObjOperator::operator=(FSmithMapObjOperator&& other) noexcept
    {
      if (this != &other)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }

    void FSmithMapObjOperator::AssignMap(TSharedPtr<FSmithMapDataModel> map, FVector originCoord_World, int32 tileSize)
    {
      m_pImpl->AssignMap(map, originCoord_World, tileSize);
    }
    void FSmithMapObjOperator::FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj, const UE::Smith::Battle::FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outActors, mapObj, format);
    }
    void FSmithMapObjOperator::MoveMapObj(ICanSetOnMap* mapObj, UE::Smith::Battle::EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
    { 
      m_pImpl->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
    }
  }
}
