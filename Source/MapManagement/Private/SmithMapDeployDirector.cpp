// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.cpp

Author : MAI ZHICONG

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapDeployDirector.h"
#include "SmithMap.h"
#include "SmithMapDataModel.h"
#include "ISmithMapEvent.h"
#include "MLibrary.h"

#include "SmithMapHelperFunc.h"

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
        void AssignMap(TSharedPtr<Model> pModel)
        {
          m_model = pModel;
        }
        void DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
        {
          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

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

          // 同じオブジェクトを二回置かないように
          if (model_shared->OnMapObjsCoordTable.Contains(mapObj)) 
          {
            return;
          }

          const uint8 mapObjSizeX = mapObj->GetOnMapSizeX();
          const uint8 mapObjSizeY = mapObj->GetOnMapSizeY();
          const uint8 mapWidth = map_shared->GetMapWidth();
          const uint8 mapHeight = map_shared->GetMapHeight();
          TArray<FMapCoord> mapObjCoords{};
          mapObjCoords.Reserve(StaticCast<int32>(mapObjSizeX * mapObjSizeY));

          // マップオブジェクトが占める座標を全て調べる
          for (uint8 mapObjLocalCoordX = 0u; mapObjLocalCoordX < mapObjSizeX; ++mapObjLocalCoordX)
          {
            for (uint8 mapObjLocalCoordY = 0u; mapObjLocalCoordY < mapObjSizeX; ++mapObjLocalCoordY)
            {
              // 座標がマップにはみ出ていないかを確認
              const int32 mapObjCheckCoordX = StaticCast<int32>(x + mapObjLocalCoordX);
              const int32 mapObjCheckCoordY = StaticCast<int32>(y + mapObjLocalCoordY);
              if (mapObjCheckCoordX >= mapWidth || mapObjCheckCoordY >= mapHeight)
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

              mapObjCoords.Emplace(mapObjOnMapCoord);
            }
          }

          for (const auto& onMapCoord : mapObjCoords)
          {
            model_shared->StaySpaceTable[onMapCoord]->SetMapObj(mapObj);
          }
          model_shared->OnMapObjsCoordTable.Emplace(mapObj, FMapCoord{x, y});
        }
        void DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y)
        {
          if (!IS_UINTERFACE_VALID(event))
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          const FMapCoord eventCoord(x, y);
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

          // TODO
          if (staySpaceTileContainer->GetEvent() == nullptr
              /*&& staySpaceTileContainer->GetMapObject() == nullptr*/)
          {
            TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
            event->SetEventCoord(x, y);

            const double eventInitLocationX = StaticCast<double>(x) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.X;
            const double eventInitLocationY = StaticCast<double>(y) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.Y;
            const double eventInitLocationZ = model_shared->OriginWorldCoord.Z;
            const FVector eventInitLocation = FVector{ eventInitLocationX, eventInitLocationY,eventInitLocationZ};

            FRotator eventRotation;
            FSmithMapHelperFunc::DirectMapElementRotation(map_shared.Get(), eventRotation, x, y);
            MDebug::LogWarning(eventRotation.ToString());
            event->InitializeEvent(eventInitLocation, eventRotation);
            staySpaceTileContainer->SetEvent(event);
          }
        }
      private:
        TWeakPtr<Model> m_model;
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
    void FSmithMapDeployDirector::AssignMap(TSharedPtr<FSmithMapDataModel> pModel)
    {
      m_pImpl->AssignMap(pModel);
    }
    void FSmithMapDeployDirector::DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y); 
    }
    void FSmithMapDeployDirector::DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y)
    {
      m_pImpl->DeployEvent(event, x, y);
    }
  } 
}
