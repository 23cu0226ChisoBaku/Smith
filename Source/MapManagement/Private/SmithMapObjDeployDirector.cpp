// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapObjDeployDirector.h"
#include "SmithMapDataModel.h"
#include "ICanSetOnMap.h"
#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMapObjDeployDirector::DeployDirectorImpl
    {
      public:
        DeployDirectorImpl()
          : m_model(nullptr)
        { }
        ~DeployDirectorImpl()
        { 
          m_model.Reset();
        }
      public:
        void DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
        {
          if (!m_model.IsValid())
          {
            return;
          }

          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return;
          }

          TSharedPtr<FSmithMapDataModel> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          for (uint8 mapObjSizeX = 0; mapObjSizeX < mapObj->GetOnMapSizeX(); ++mapObjSizeX)
          {
            for (uint8 mapObjSizeY = 0; mapObjSizeY < mapObj->GetOnMapSizeY(); ++mapObjSizeY)
            {

              FMapCoord coord(x + mapObjSizeX, y + mapObjSizeY);
              if (!model_shared->StaySpaceTable.Contains(coord))
              {
                MDebug::LogError("Can not place Obj Here");
                return;
              }

              model_shared->StaySpaceTable[coord]->SetMapObj(mapObj);
            }
          }

          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            model_shared->OnMapObjsCoordTable.Emplace(mapObj, FMapCoord{x, y});
          }

          MDebug::LogWarning("Deploy map obj");
        }
        void AssignMap(TSharedPtr<FSmithMapDataModel> pModel)
        {
          m_model = pModel;
        }
      private:
        TWeakPtr<FSmithMapDataModel> m_model;
    };
    FSmithMapObjDeployDirector::FSmithMapObjDeployDirector()
      : m_pImpl(::MakeUnique<DeployDirectorImpl>())
    { }

    FSmithMapObjDeployDirector::~FSmithMapObjDeployDirector()
    {
      m_pImpl.Reset();
    }

    FSmithMapObjDeployDirector::FSmithMapObjDeployDirector(FSmithMapObjDeployDirector&& other) noexcept
      : m_pImpl(::MoveTemp(other.m_pImpl))
    { }

    FSmithMapObjDeployDirector& FSmithMapObjDeployDirector::operator=(FSmithMapObjDeployDirector&& other) noexcept
    {
      if (this != &other)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }

    void FSmithMapObjDeployDirector::AssignMap(TSharedPtr<FSmithMapDataModel> pModel)
    {
      m_pImpl->AssignMap(pModel);
    }

    void FSmithMapObjDeployDirector::DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y); 
    }
  } 
}
