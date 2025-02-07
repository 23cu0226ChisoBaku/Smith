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
#include "SmithMapDataModel.h"
#include "ICanSetOnMap.h"
#include "ISmithMapEvent.h"
#include "MLibrary.h"

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
          check(m_model.IsValid())
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

          for (uint8 mapObjSizeX = 0; mapObjSizeX < mapObj->GetOnMapSizeX(); ++mapObjSizeX)
          {
            for (uint8 mapObjSizeY = 0; mapObjSizeY < mapObj->GetOnMapSizeY(); ++mapObjSizeY)
            {

              const FMapCoord coord(x + mapObjSizeX, y + mapObjSizeY);
              if (!model_shared->StaySpaceTable.Contains(coord))
              {
                MDebug::LogError("Can not place Obj Here");
                return;
              }

              model_shared->StaySpaceTable[coord]->SetMapObj(mapObj);
            }
          }

          // マップに存在しないオブジェクトを追加する
          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            model_shared->OnMapObjsCoordTable.Emplace(mapObj, FMapCoord{x, y});
          }
        }
        void DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y, const FRotator& rotation)
        {
          check(m_model.IsValid())
          if (!m_model.IsValid())
          {
            return;
          }

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

          const auto& staySpaceTileContainer = model_shared->StaySpaceTable[eventCoord];

          // TODO
          if (staySpaceTileContainer->GetEvent() == nullptr
              /*&& staySpaceTileContainer->GetMapObject() == nullptr*/)
          {
            model_shared->StaySpaceTable[eventCoord]->SetEvent(event);
            const double eventInitLocationX = StaticCast<double>(x) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.X;
            const double eventInitLocationY = StaticCast<double>(y) * StaticCast<double>(model_shared->MapTileSize) + model_shared->OriginWorldCoord.Y;
            const double eventInitLocationZ = model_shared->OriginWorldCoord.Z;
            const FVector eventInitLocation = FVector{ eventInitLocationX, eventInitLocationY,eventInitLocationZ};

            event->SetEventCoord(x, y);
            event->InitializeEvent(eventInitLocation, rotation);
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

    void FSmithMapDeployDirector::DeployEvent(ISmithMapEvent* event, uint8 x, uint8 y, const FRotator& rotation)
    {
      m_pImpl->DeployEvent(event, x, y, rotation);
    }
  } 
}
