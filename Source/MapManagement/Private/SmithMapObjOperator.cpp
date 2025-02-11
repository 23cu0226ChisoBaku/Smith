// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjOperator.h

Author : MAI ZHICONG(バクチソウ)

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
#include "IEventRegister.h"
#include "ISmithMapEvent.h"
#include "SmithCommandFormat.h"
#include "FormatType.h"
#include "FormatTransformer.h"
#include "MapObjType.h"

// TODO
// IAttackable.h
// Direction.h
#include "AttackableInfoHandle.h"
#include "SmithModelHelperFunctionLibrary.h"

#include "UObject/WeakInterfacePtr.h"
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
          , m_eventRegister(nullptr)
        { }
        ~MapObjOperatorImpl()
        { }

        void AssignMap(TSharedPtr<Model> model)
        {
          m_model = model;
        }
        void AssignEventRegister(IEventRegister* eventRegister)
        {
          m_eventRegister = eventRegister;
        }
        void FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          // 安全性チェック
          #pragma region Safe Check
          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          if (format.GetRow() == 0 || format.GetColumn() == 0)
          {
            return;
          }
          #pragma endregion Safe Check
          // end of Safe Check

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            return;
          }
          outAttackableHandles.Reset();

          const uint8 mapObjSizeX = mapObj->GetOnMapSizeX();
          const uint8 mapObjSizeY = mapObj->GetOnMapSizeY();
          for (uint8 coordX = 0u; coordX < mapObjSizeX; ++coordX)
          {
            for (uint8 coordY = 0u; coordY < mapObjSizeY; ++coordY)
            {
              // オブジェクトの中心座標
              const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj] + FMapCoord(coordX, coordY);

              const EMapObjType trackerType = mapObj->GetType();

              // 攻撃フォーマットをマップオブジェクトの中心座標でマップ座標に変換
              using namespace UE::MLibrary::MDataStructure;
              TDimension2Array<FMapCoord> mapCoords = FFormatTransformer::FormatToMapCoord(format, mapObjOriginCoord);

              for (uint64 y = 0; y < format.GetRow(); ++y)
              {
                for (uint64 x = 0; x < format.GetColumn(); ++x)
                {
                  ESmithFormatType formatType = format.GetFormatData(x, y);
                  // 効果がないマスだったら無視する
                  if (formatType != ESmithFormatType::EFFECT)
                  {
                    continue;
                  }

                  // マス座標がものを置ける座標じゃないと飛ばす
                  const FMapCoord mapCoord = mapCoords.At_ReadOnly(y, x);
                  if (!model_shared->StaySpaceTable.Contains(mapCoord))
                  {
                    continue;
                  }

                  // 座標にマップオブジェクトがないと飛ばす
                  ICanSetOnMap* coordMapObj = model_shared->StaySpaceTable[mapCoord]->GetMapObject();
                  if (coordMapObj == nullptr || coordMapObj == mapObj)
                  {
                    continue;
                  }

                  // 同じタイプのターゲットに攻撃しない
                  if (coordMapObj->GetType() == trackerType)
                  {
                    continue;
                  }

                  // TODO Safe Cast may cause performance issue
                  // マップオブジェクトが攻撃できないと飛ばす
                  IAttackable* attackable = Cast<IAttackable>(coordMapObj);
                  if (!IS_UINTERFACE_VALID(attackable))
                  {
                    continue;
                  }

                  const EDirection attackFrom = FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(model_shared->OnMapObjsCoordTable[mapObj], model_shared->OnMapObjsCoordTable[coordMapObj]);

                  outAttackableHandles.Add(FAttackableInfoHandle{attackable, attackFrom});
                }
              }

              if (outAttackableHandles.Num() > 0)
              {
                break;
              }
            }
            
            if (outAttackableHandles.Num() > 0)
            {
              break;
            } 
          }
        }
        void FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const UE::Smith::Battle::FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
        {
          // 安全性チェック
          #pragma region Safe Check
          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          if (format.GetRow() == 0 || format.GetColumn() == 0)
          {
            return;
          }
          #pragma endregion Safe Check
          // end of Safe Check

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            return;
          }

          outAttackableHandles.Reset();

          const uint8 mapObjSizeX = mapObj->GetOnMapSizeX();
          const uint8 mapObjSizeY = mapObj->GetOnMapSizeY();

          const uint8 coordX = mapObjSizeX > offsetToLeft ? offsetToLeft : 0u;
          const uint8 coordY = mapObjSizeY > offsetToTop ? offsetToTop : 0u;
          // オブジェクトの中心座標
          const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj] + FMapCoord(coordX, coordY);

          const EMapObjType trackerType = mapObj->GetType();

          // 攻撃フォーマットをマップオブジェクトの中心座標でマップ座標に変換
          using namespace UE::MLibrary::MDataStructure;
          TDimension2Array<FMapCoord> mapCoords = FFormatTransformer::FormatToMapCoord(format, mapObjOriginCoord);

          for (uint64 y = 0; y < format.GetRow(); ++y)
          {
            for (uint64 x = 0; x < format.GetColumn(); ++x)
            {
              ESmithFormatType formatType = format.GetFormatData(x, y);
              // 効果がないマスだったら無視する
              if (formatType != ESmithFormatType::EFFECT)
              {
                continue;
              }

              // マス座標がものを置ける座標じゃないと飛ばす
              const FMapCoord mapCoord = mapCoords.At_ReadOnly(y, x);
              if (!model_shared->StaySpaceTable.Contains(mapCoord))
              {
                continue;
              }

              // 座標にマップオブジェクトがないと飛ばす
              ICanSetOnMap* coordMapObj = model_shared->StaySpaceTable[mapCoord]->GetMapObject();
              if (coordMapObj == nullptr || coordMapObj == mapObj)
              {
                continue;
              }

              if (coordMapObj->GetType() == trackerType)
              {
                continue;
              }

              // TODO Safe Cast may cause performance issue
              // マップオブジェクトが攻撃できないと飛ばす
              IAttackable* attackable = Cast<IAttackable>(coordMapObj);
              if (!IS_UINTERFACE_VALID(attackable))
              {
                continue;
              }

              const EDirection attackFrom = FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(model_shared->OnMapObjsCoordTable[mapObj], model_shared->OnMapObjsCoordTable[coordMapObj]);

              outAttackableHandles.Add(FAttackableInfoHandle{attackable, attackFrom});
            }
          }
          
        }
        void MoveMapObj(ICanSetOnMap* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          destination = InvalidValues::MapInvalidCoord_World;
          // 安全性チェック
          #pragma region Safe Check
          {
            check(m_model.IsValid())
            if (!m_model.IsValid())
            {
              return;
            }

            if (!IS_UINTERFACE_VALID(mapObj) || moveDistance == 0)
            {
              return;
            }
          }
          #pragma endregion Safe Check
          // end of Safe Check

          const uint8 moveDirectionCheck_bitsIdx = StaticCast<uint8>(moveDirection);
          if (moveDirectionCheck_bitsIdx >= StaticCast<uint8>(EDirection::DirectionCount))
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          using namespace Private;

          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            return;
          }

          const uint8 directionCheck_bits = DirectionCheckBits[moveDirectionCheck_bitsIdx];

          // 実際の移動距離
          uint8 actualMoveDistance = 0;
          // 離れた所から探していく
          for (uint8 checkMoveDistance = 1; checkMoveDistance <= moveDistance; ++checkMoveDistance)
          {
            if (isObjectMoveable(mapObj, checkMoveDistance, directionCheck_bits))
            {
              ++actualMoveDistance;
            }
          }
          updateObjCoord(mapObj, directionCheck_bits, actualMoveDistance, destination);
        }
      private:
        bool isObjectMoveable(ICanSetOnMap* mapObj, uint8 distance, uint8 checkBits_Internal) const
        {
          // 安全性チェック(二重チェックのためDebug専用)
          #if WITH_EDITOR
            #pragma region Safe Check
              check(IS_UINTERFACE_VALID(mapObj));
              check(m_model.IsValid());
            #pragma endregion Safe Check
          #endif

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

          // 行き先座標取得
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
          // 安全性チェック(二重チェックのためDebug専用)
          #if WITH_EDITOR
            #pragma region Safe Check
            {
              check(IS_UINTERFACE_VALID(mapObj));
              check(m_model.IsValid());
            }
            #pragma endregion Safe Check
          #endif

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
          
          if (distanceX == 0 && distanceY == 0)
          {
            return;
          }

          const FMapCoord mapObjOriginCoord = model_shared->OnMapObjsCoordTable[mapObj];
          
          // TODO ignore ground exist check;
          for (uint8 y = 0; y < mapObj->GetOnMapSizeY(); ++y)
          {
            for (uint8 x = 0; x < mapObj->GetOnMapSizeX(); ++x)
            {
              // マップオブジェクトを移動する前のタイルの座標
              const FMapCoord removeTileCoord(mapObjOriginCoord.x + x, mapObjOriginCoord.y + y);

              const int32 fillTileCoordX = StaticCast<int32>(removeTileCoord.x) + distanceX;
              const int32 fillTileCoordY = StaticCast<int32>(removeTileCoord.y) + distanceY;
              // マップオブジェクトを移動した後のタイルの座標
              const FMapCoord fillTileCoord(StaticCast<uint8>(fillTileCoordX), StaticCast<uint8>(fillTileCoordY));

              model_shared->StaySpaceTable[removeTileCoord]->SetMapObj(nullptr);
              model_shared->StaySpaceTable[fillTileCoord]->SetMapObj(mapObj);

              if (m_eventRegister.IsValid())
              {
                ISmithMapEvent* event = model_shared->StaySpaceTable[fillTileCoord]->GetEvent();
                if (IS_UINTERFACE_VALID(event))
                {
                  m_eventRegister->RegisterMapEvent(mapObj, event);
                }
              }
            }
          }

          // TODO マップオブジェクトの中心座標を更新
          const int32 newMapObjOriginCoordX = StaticCast<int32>(mapObjOriginCoord.x) + distanceX;
          const int32 newMapObjOriginCoordY = StaticCast<int32>(mapObjOriginCoord.y) + distanceY;
          const FMapCoord newMapObjOriginCoord(StaticCast<uint8>(newMapObjOriginCoordX), StaticCast<uint8>(newMapObjOriginCoordY));
          model_shared->OnMapObjsCoordTable[mapObj] = newMapObjOriginCoord;
          
          const uint8 mapSizeX = mapObj->GetOnMapSizeX();
          const uint8 mapSizeY = mapObj->GetOnMapSizeY();
          const FVector mapObjoffset = FVector(
                                      StaticCast<double>((mapSizeX - 1u) * model_shared->MapTileSize) * 0.5,
                                      StaticCast<double>((mapSizeY - 1u) * model_shared->MapTileSize) * 0.5,
                                      0.0
                                    );
          // TODO
          destination = model_shared->OriginWorldCoord 
                      + FVector(
                                newMapObjOriginCoordX * model_shared->MapTileSize, 
                                newMapObjOriginCoordY * model_shared->MapTileSize, 
                                0.0
                                )
                      + mapObjoffset;

        }
      private:
        TWeakPtr<Model> m_model;
        TWeakInterfacePtr<IEventRegister> m_eventRegister;
    };

    FSmithMapObjOperator::FSmithMapObjOperator()
      : m_pImpl(::MakeUnique<MapObjOperatorImpl>())
    { }

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
    void FSmithMapObjOperator::AssignEventRegister(IEventRegister* eventRegister)
    {
      m_pImpl->AssignEventRegister(eventRegister);
    }
    void FSmithMapObjOperator::AssignMap(TSharedPtr<FSmithMapDataModel> map)
    {
      m_pImpl->AssignMap(map);
    }
    void FSmithMapObjOperator::FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const UE::Smith::Battle::FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outAttackableHandles, mapObj, format);
    }
    void FSmithMapObjOperator::FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const UE::Smith::Battle::FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
    {
      m_pImpl->FindAttackableMapObjsFromCoord(outAttackableHandles, mapObj, format, offsetToLeft, offsetToTop);
    }
    void FSmithMapObjOperator::MoveMapObj(ICanSetOnMap* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
    { 
      m_pImpl->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
    }
  }
}
