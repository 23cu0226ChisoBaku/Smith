// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithMapManager.h"
#include "SmithRect.h"
#include "SmithMap.h"

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

#include "SmithMapBuilder.h"
#include "SmithMapConstructor.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithMapObjOperator.h"
#include "SmithMapObjDeployDirector.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "SmithMapObserver.h"
#include "SmithMapDataModel.h"

#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    using namespace UE::Smith::Battle;

    class FSmithMapManager::MapMgrImpl
    {
      public:
        MapMgrImpl()
          : m_mapBuilder(::MakeUnique<FSmithMapBuilder>())
          , m_mapConstructor(::MakeUnique<FSmithMapConstructor>())
          , m_mapOperator(::MakeUnique<FSmithMapObjOperator>())
          , m_deployDirector(::MakeUnique<FSmithMapObjDeployDirector>())
          , m_mapObserver(::MakeUnique<FSmithMapObserver>())
          , m_model(::MakeShared<FSmithMapDataModel>())
          , m_map(nullptr)
        { }
        ~MapMgrImpl()
        { 
          m_model.Reset();
          m_map.Reset();
        }
        void InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
        {    

          check(::IsValid(world));
          if (world == nullptr)
          {
            return;
          }

          TSharedPtr<FSmithMap> temp = ::MakeShared<FSmithMap>();
          
          if(!m_mapBuilder->Build(temp.Get(), mapBP))
          {
            return;
          }

          TSharedPtr<FSmithMapDataModel> tempModel = m_mapBuilder->GenerateModel(temp);
          
          check(tempModel.IsValid());
          if (!tempModel.IsValid())
          {
            return;
          }

          m_map.Reset();
          m_map = ::MoveTemp(temp);

          m_model.Reset();
          m_model = ::MoveTemp(tempModel);

          m_mapConstructor->ConstructMap(world, m_map->GetMap(), constructionBP);

          m_mapOperator->AssignMap(m_model, constructionBP.OriginCoordinate, constructionBP.TileSize);
          m_deployDirector->AssignMap(m_model);
          m_mapObserver->AssignMap(m_model, constructionBP.OriginCoordinate, constructionBP.TileSize);
        }
        void InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
        {
          check((m_mapObserver.IsValid() && m_deployDirector.IsValid()));
          if (!m_mapObserver.IsValid() || !m_deployDirector.IsValid())
          {
            return;
          }

          check(::IsValid(world) && ::IsValid(player));
          if (world == nullptr || player == nullptr)
          {
            return;
          }

          TMap<FMapCoord, ICanSetOnMap*> deployMapObjs{};
          m_mapObserver->InitMapObj(deployMapObjs, world, player, generateBP);

          for(const auto& deployPair : deployMapObjs)
          {
            m_deployDirector->DeployMapObj(deployPair.Value, deployPair.Key.x, deployPair.Key.y);
          }

        }
        void DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
        {
          m_deployDirector->DeployMapObj(mapObj, x, y);
        }
        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          m_mapOperator->FindAttackableMapObjs(outActors, mapObj, format);
        }
        void MoveMapObj(ICanSetOnMap* mapObj, EMoveDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          m_mapOperator->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
        }
      private:
        TUniquePtr<FSmithMapBuilder> m_mapBuilder;
        TUniquePtr<FSmithMapConstructor> m_mapConstructor;
        TUniquePtr<FSmithMapObjOperator> m_mapOperator;
        TUniquePtr<FSmithMapObjDeployDirector> m_deployDirector;
        TUniquePtr<FSmithMapObserver> m_mapObserver;
        TSharedPtr<FSmithMapDataModel> m_model;
        TSharedPtr<FSmithMap> m_map;
    };
    FSmithMapManager::FSmithMapManager()
      : m_pImpl(::MakeUnique<MapMgrImpl>())
    { }
    FSmithMapManager::~FSmithMapManager()
    {
      m_pImpl.Reset();
    }
    void FSmithMapManager::InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
    {
      m_pImpl->InitMap(world, mapBP, constructionBP);
    }
    void FSmithMapManager::InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
    {
      m_pImpl->InitMapObjs(world, player, generateBP);
    }
    void FSmithMapManager::DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y);
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
