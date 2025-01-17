// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.cpp

Author : MAI ZHICONG

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapObjDeployDirector.h"
#include "SmithMapDataModel.h"
#include "ICanSetOnMap.h"
#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithMapObjDeployDirector実装クラス
    /// namespace UE::Smith::Map
    ///
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
        void AssignMap(TSharedPtr<FSmithMapDataModel> pModel)
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

          TSharedPtr<FSmithMapDataModel> model_shared = m_model.Pin();
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
