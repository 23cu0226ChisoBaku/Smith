// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/08 作成
								2025/01/20 イベント配置インターフェース追加
								2025/01/21 イベントの回転パラメーターを追加
                2025/02/05 イベントの回転を決めるヘルパークラス追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapDeployDirector.h"
#include "SmithMap.h"
#include "SmithMapDataModel.h"
#include "ISmithMapEvent.h"
#include "SmithMapHelperLibrary.h"
#include "MLibrary.h"
#include "IMinimapDisplayable.h"

#include "ISmithMapModelRequester.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithMapDeployDirector
    /// namespace UE::Smith::Map
    ///
    class FSmithMapDeployDirector::DeployDirectorImpl
    {
      using Model = typename FSmithMapDataModel;
      using Map = typename FSmithMap;
      public:
        DeployDirectorImpl()
          : m_model(nullptr)
        { }
        ~DeployDirectorImpl()
        { 
          m_model.Reset();
        }
      public:
        void AssignMapDataModel(TSharedPtr<Model> pModel)
        {
          m_model = pModel;
        }
        void DeployMapObj(AActor* mapObj, uint8 x, uint8 y)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          TSharedPtr<Map> map_shared = model_shared->Map.Pin();
          if (!map_shared.IsValid())
          {
            return;
          }

          if (mapObj == nullptr)
          {
            return;
          }

          // 同じオブジェクトを二回置かないように
          if (model_shared->OnMapObjsCoordTable.Contains(mapObj)) 
          {
            return;
          }

          if (!model_shared->MapModelRequester.IsValid())
          {
            return;
          }

          const FSmithMapModel model = model_shared->MapModelRequester->GetModel(Cast<AActor>(mapObj));
          if (!model.IsValid())
          {
            return;
          }

        
          const uint8 mapObjSizeX = model.GetSizeX();
          const uint8 mapObjSizeY = model.GetSizeY();
          const uint8 mapWidth = map_shared->GetMapWidth();
          const uint8 mapHeight = map_shared->GetMapHeight();
          TArray<FMapCoord> mapObjPlaceCoords{};
          mapObjPlaceCoords.Reserve(StaticCast<int32>(mapObjSizeX) * StaticCast<int32>(mapObjSizeY));

          // マップオブジェクトが占める座標を全て調べる
          for (uint8 mapObjLocalCoordX = 0u; mapObjLocalCoordX < mapObjSizeX; ++mapObjLocalCoordX)
          {
            for (uint8 mapObjLocalCoordY = 0u; mapObjLocalCoordY < mapObjSizeX; ++mapObjLocalCoordY)
            {
              // 座標がマップにはみ出ていないかを確認
              const int32 mapObjCheckCoordX = StaticCast<int32>(x) + StaticCast<int32>(mapObjLocalCoordX);
              const int32 mapObjCheckCoordY = StaticCast<int32>(y) + StaticCast<int32>(mapObjLocalCoordY);
              if (mapObjCheckCoordX >= StaticCast<int32>(mapWidth) || mapObjCheckCoordY >= StaticCast<int32>(mapHeight))
              {
                return;
              }

              const FMapCoord mapObjOnMapCoord(x + mapObjLocalCoordX, y + mapObjLocalCoordY);
              // オブジェクトが配置できないと関数から抜ける
              if (!model_shared->StaySpaceTable.Contains(mapObjOnMapCoord))
              {
                MDebug::LogError("Can not place Obj Here");
                return;
              }

              mapObjPlaceCoords.Emplace(mapObjOnMapCoord);
            }
          }

          // タイル情報にマップオブジェクトをいれる
          for (const auto& placeCoord : mapObjPlaceCoords)
          {
            model_shared->StaySpaceTable[placeCoord]->SetMapObj(mapObj);
          }
          // マップオブジェクトの原点座標だけテーブルに記録する
          const FMapCoord placeOriginCoord = FMapCoord{x, y};
          model_shared->OnMapObjsCoordTable.Emplace(mapObj, placeOriginCoord);

          if (m_updateMinimapDelegate.IsBound())
          {
            m_updateMinimapDelegate.Broadcast(mapObj, placeOriginCoord, mapObjSizeX, mapObjSizeY);
          }
        }
        void DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          if (!IS_UINTERFACE_VALID(event))
          {
            return;
          }

          const FMapCoord eventCoord(x, y);
          // 座標にイベントを配置できるか確認
          if (!model_shared->StaySpaceTable.Contains(eventCoord))
          {
            MDebug::LogError("cant place event here");
            return;
          }

          TSharedPtr<FStaySpaceTileInfoContainer> staySpaceTileContainer = model_shared->StaySpaceTable[eventCoord];
          if (!staySpaceTileContainer.IsValid())
          {
            return;
          }

          // 座標にイベントが配置されていない場合だけイベントを配置する
          if (staySpaceTileContainer->GetEvent() == nullptr)
          {
            FRotator eventRotation;
            TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();

            // イベントの向きを決める
            FSmithMapHelperLibrary::DirectMapElementRotation(map_shared.Get(), eventRotation, x, y);

            const double eventLocationX = StaticCast<double>(x) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.X;
            const double eventLocationY = StaticCast<double>(y) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.Y;
            const double eventLocationZ = model_shared->OriginWorldCoord.Z;
            const FVector eventLocation = FVector{ eventLocationX, eventLocationY,eventLocationZ};

            // イベント初期化
            event->InitializeEvent(eventLocation, eventRotation);
            staySpaceTileContainer->SetEvent(event);

            if (m_updateMinimapDelegate.IsBound())
            {
              m_updateMinimapDelegate.Broadcast(event->_getUObject(), FMapCoord(x, y), 1u, 1u);
            }
          }
        }
        void AddUpdateMinimapDelegate(const TDelegate<void(UObject*, const FMapCoord&, uint8, uint8)>& delegate)
        {
          if (!delegate.IsBound())
          {
            return;
          }

          m_updateMinimapDelegate.Add(delegate);
        }
      private:
        TWeakPtr<Model> m_model;
        TMulticastDelegate<void(UObject*, const FMapCoord&, uint8, uint8)> m_updateMinimapDelegate;
    };
    FSmithMapDeployDirector::FSmithMapDeployDirector()
      : m_pImpl(::MakeUnique<DeployDirectorImpl>())
    { }
    FSmithMapDeployDirector::~FSmithMapDeployDirector()
    {
      m_pImpl.Reset();
    }
    FSmithMapDeployDirector::FSmithMapDeployDirector(FSmithMapDeployDirector&& other) noexcept
      : m_pImpl(::MoveTemp(other.m_pImpl))
    { }
    FSmithMapDeployDirector& FSmithMapDeployDirector::operator=(FSmithMapDeployDirector&& other) noexcept
    {
      if (this != &other)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }
    void FSmithMapDeployDirector::AssignMapDataModel(TSharedPtr<FSmithMapDataModel> pModel)
    {
      m_pImpl->AssignMapDataModel(pModel);
    }
    void FSmithMapDeployDirector::DeployMapObj(AActor* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y); 
    }
    void FSmithMapDeployDirector::DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y)
    {
      m_pImpl->DeployEvent(event, x, y);
    }
    void FSmithMapDeployDirector::AddUpdateMinimapDelegate(const TDelegate<void(UObject*, const FMapCoord&, uint8, uint8)>& delegate)
    {
      m_pImpl->AddUpdateMinimapDelegate(delegate);
    }
  } 
}
