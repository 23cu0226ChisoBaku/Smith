// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapManager.cpp

Author : MAI ZHICONG

Description : マップマネージャー

Update History: 2025/01/04 作成
              : 2025/01/06 クラス分けてマップを操作する

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#include "SmithMapManager.h"
#include "SmithRect.h"
#include "SmithMap.h"

#include "ICanSetOnMap.h"
#include "IAttackable.h"

#include "MapCoord.h"
#include "TileType.h"
#include "SmithCommandFormat.h"
#include "Direction.h"

#include "SmithMapBuilder.h"
#include "SmithMapConstructor.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithMapObjOperator.h"
#include "SmithMapDeployDirector.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "SmithMapObserver.h"
#include "SmithMapDataModel.h"
#include "SmithEventPublisher.h"
#include "IEventRegister.h"
#include "Kismet/GameplayStatics.h"

#include "SmithNextLevelEvent.h"

#include "MLibrary.h"

// TODO
#include "MapGenerateGameMode_Test.h"

namespace UE::Smith
{
  namespace Map
  {
    using namespace UE::Smith::Battle;

    ///
    /// @brief マップマネージャー実装クラス
    ///
    class FSmithMapManager::MapMgrImpl
    {
      using Model = typename FSmithMapDataModel;
      public:
        //---------------------------------------
        /*
                        ctorとdtor
        */
        //---------------------------------------
        MapMgrImpl()
          : m_mapBuilder(::MakeUnique<FSmithMapBuilder>())
          , m_mapConstructor(::MakeUnique<FSmithMapConstructor>())
          , m_mapOperator(::MakeUnique<FSmithMapObjOperator>())
          , m_deployDirector(::MakeUnique<FSmithMapDeployDirector>())
          , m_mapObserver(::MakeUnique<FSmithMapObserver>())
          , m_model(::MakeShared<Model>())
          , m_map(nullptr)
          , m_mapEvents{}
        { }
        ~MapMgrImpl()
        { 
          m_model.Reset();
          m_map.Reset();
        }
        void AssignEventRegister(IEventRegister* eventRegister)
        {
          check(m_mapOperator.IsValid())
          if (!m_mapOperator.IsValid())
          {
            return;
          }

          m_mapOperator->AssignEventRegister(eventRegister);
        }
        void InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
        {    
          // 安全性チェック
          #pragma region Safe Check
            check(::IsValid(world));
            if (world == nullptr)
            {
              return;
            }
          #pragma endregion Safe Check

          TSharedPtr<FSmithMap> temp = ::MakeShared<FSmithMap>();
          
          // マップを構築する
          if(!m_mapBuilder->Build(temp.Get(), mapBP))
          {
            return;
          }

          // マップモデルを作成する
          TSharedPtr<Model> tempModel = m_mapBuilder->GenerateModel(temp);
          check(tempModel.IsValid());
          if (!tempModel.IsValid())
          {
            return;
          }

          tempModel->OriginWorldCoord = constructionBP.OriginCoordinate;
          tempModel->MapTileSize = constructionBP.TileSize;

          // マネージャーを更新
          m_map.Reset();
          m_map = ::MoveTemp(temp);

          m_model.Reset();
          m_model = ::MoveTemp(tempModel);

          m_mapConstructor->ConstructMap(world, m_map->GetMap(), constructionBP);

          m_mapOperator->AssignMap(m_model, constructionBP.OriginCoordinate, constructionBP.TileSize);
          m_deployDirector->AssignMap(m_model);
          m_mapObserver->AssignMap(m_model, constructionBP.OriginCoordinate, constructionBP.TileSize);

          for (auto& event : m_mapEvents)
          {
            if (IS_UINTERFACE_VALID(event))
            {
              event->DiscardEvent();
            }
          }

          m_mapEvents.Reset();
        }
        void InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
        {
          // 安全性チェック
          #pragma region Safe Check
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
          #pragma endregion Safe Check

          TMap<FMapCoord, ICanSetOnMap*> deployMapObjs{};
          m_mapObserver->InitMapObj(deployMapObjs, world, player, generateBP);

          for(const auto& deployPair : deployMapObjs)
          {
            m_deployDirector->DeployMapObj(deployPair.Value, deployPair.Key.x, deployPair.Key.y);
          }

        }
        void InitMapEvents(UWorld* world, USmithEventPublisher* eventPublisher)
        {
          // TODO test event
          check((m_mapObserver.IsValid() && m_deployDirector.IsValid()));
          if (!m_mapObserver.IsValid() || !m_deployDirector.IsValid())
          {
            return;
          }

          if (!::IsValid(world) || !::IsValid(eventPublisher))
          {
            return;
          }
          auto nextLevelEvent = eventPublisher->PublishMapEvent<USmithNextLevelEvent>(USmithNextLevelEvent::StaticClass());
          if (nextLevelEvent == nullptr)
          {
            MDebug::LogError("Publish failed");
          }
          else
          {
            nextLevelEvent->OnNextLevel.BindLambda(
              [=](){
                AMapGenerateGameMode_Test* testGameMode = Cast<AMapGenerateGameMode_Test>(world->GetAuthGameMode());
                if (testGameMode != nullptr)
                {
                  testGameMode->goToNextLevel();
                }
              }
            );
            uint8 nextLevelEventCoordX = 0;
            uint8 nextLevelEventCoordY = 0;
            FVector nextLevelEventDestination = FVector::ZeroVector;

            m_mapObserver->InitNextLevelEvent_Temp(nextLevelEventCoordX, nextLevelEventCoordY, nextLevelEventDestination);
            m_deployDirector->DeployEvent(nextLevelEvent, nextLevelEventCoordX, nextLevelEventCoordY);

            m_mapEvents.Emplace(nextLevelEvent);
          }
        }

        void DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
        {
          m_deployDirector->DeployMapObj(mapObj, x, y);
        }
        void DeployEvent(ISmithMapEvent* mapEvent, uint8 x, uint8 y)
        {
          m_deployDirector->DeployEvent(mapEvent, x, y);
          
          if (IS_UINTERFACE_VALID(mapEvent))
          {
            m_mapEvents.Emplace(mapEvent);
          }
        }
        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          m_mapOperator->FindAttackableMapObjs(outActors, mapObj, format);
        }
        void MoveMapObj(ICanSetOnMap* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          m_mapOperator->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
        }
        bool ChasePlayerTarget(EDirection& outChaseDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
        {
          return m_mapObserver->ChasePlayer(outChaseDirection, chaser, chaseRadius);
        }
        bool GetMapObjectCoord(ICanSetOnMap* mapObj, uint8& x, uint8& y)
        {
          return m_mapObserver->GetMapObjectCoord(mapObj, x, y);
        }
        void Reset()
        {
          m_mapObserver->ClearMapObjs_IgnorePlayer();
          m_mapConstructor->DestructMap();
        }
        void AddReferencedObjects(FReferenceCollector& collector)
        {
          int32 idx = 0;
          
          while (idx < m_mapEvents.Num())
          {
            if (!IS_UINTERFACE_VALID(m_mapEvents[idx]))
            {
              m_mapEvents.RemoveAt(idx);
              continue;
            }

            const UObject* eventUObject = m_mapEvents[idx]->_getUObject();
            collector.AddReferencedObject(eventUObject);
            ++idx;
          }
        }
      private:
        TUniquePtr<FSmithMapBuilder> m_mapBuilder;
        TUniquePtr<FSmithMapConstructor> m_mapConstructor;
        TUniquePtr<FSmithMapObjOperator> m_mapOperator;
        TUniquePtr<FSmithMapDeployDirector> m_deployDirector;
        TUniquePtr<FSmithMapObserver> m_mapObserver;
        TSharedPtr<Model> m_model;
        TSharedPtr<FSmithMap> m_map;
        TArray<ISmithMapEvent*> m_mapEvents;
    };
    FSmithMapManager::FSmithMapManager()
      : m_pImpl(::MakeUnique<MapMgrImpl>())
    { }
    FSmithMapManager::~FSmithMapManager()
    {
      m_pImpl.Reset();
    }
    void FSmithMapManager::AssignEventRegister(IEventRegister* eventRegister)
    {
      m_pImpl->AssignEventRegister(eventRegister);
    }
    void FSmithMapManager::InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
    {
      m_pImpl->InitMap(world, mapBP, constructionBP);
    }
    void FSmithMapManager::InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
    {
      m_pImpl->InitMapObjs(world, player, generateBP);
    }
    void FSmithMapManager::InitMapEvents(UWorld* world, USmithEventPublisher* eventPublisher)
    {
      m_pImpl->InitMapEvents(world, eventPublisher);
    }
    void FSmithMapManager::DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y);
    }
    void FSmithMapManager::DeployEvent(ISmithMapEvent* mapEvent, uint8 x, uint8 y)
    {
      m_pImpl->DeployEvent(mapEvent, x, y);
    }
    void FSmithMapManager::FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap* mapObj,const FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outActors, mapObj, format);
    } 
    void FSmithMapManager::MoveMapObj(ICanSetOnMap* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
    {
      return m_pImpl->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
    }
    bool FSmithMapManager::ChasePlayerTarget(EDirection& outChaseDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
    {
      return m_pImpl->ChasePlayerTarget(outChaseDirection, chaser, chaseRadius);
    }
    bool FSmithMapManager::GetMapObjectCoord(ICanSetOnMap* mapObj, uint8& x, uint8& y)
    {
      return m_pImpl->GetMapObjectCoord(mapObj, x, y);
    }
    void FSmithMapManager::Reset()
    {
      m_pImpl->Reset();
    }
    void FSmithMapManager::AddReferencedObjects(FReferenceCollector& collector)
    {
      m_pImpl->AddReferencedObjects(collector);
    }
    FString FSmithMapManager::GetReferencerName() const
    {
      return TEXT("Smith Map Manager");
    }
  }
}
