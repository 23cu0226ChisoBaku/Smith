// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapManager.cpp

Author : MAI ZHICONG(バクチソウ)

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

#include "MapCoord.h"
#include "TileType.h"
#include "SmithCommandFormat.h"

#include "SmithMapBuilder.h"
#include "SmithMapConstructor.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithMapObjOperator.h"
#include "SmithMapDeployDirector.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "SmithMapObserver.h"
#include "SmithMapEventDirector.h"
#include "SmithMapDataModel.h"
#include "SmithEventPublisher.h"
#include "IEventRegister.h"

// TODO
#include "AttackableInfoHandle.h"
#include "ItemGenerationListRow.h"
#include "SmithModelHelperFunctionLibrary.h"
#include "SmithMapDecoration.h"

// TODO マネージャーは仕事をしない筈
#include "SmithMapHelperLibrary.h"

#include "MLibrary.h"

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
          , m_eventDirector(::MakeUnique<FSmithMapEventDirector>())
          , m_mapObserver(::MakeUnique<FSmithMapObserver>())
          , m_model(::MakeShared<Model>())
          , m_map(nullptr)
          , m_mapEvents{}
        { }
        ~MapMgrImpl()
        { 
          m_mapBuilder.Reset();
          m_mapConstructor.Reset();
          m_mapOperator.Reset();
          m_deployDirector.Reset();
          m_eventDirector.Reset();
          m_mapObserver.Reset();
          m_model.Reset();
          m_map.Reset();

          discardEvents();
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

          TSharedPtr<FSmithMap> tempMap = ::MakeShared<FSmithMap>();
          
          // マップを構築する
          if(!m_mapBuilder->Build(tempMap, mapBP))
          {
            return;
          }

          // マップモデルを作成する
          TSharedPtr<Model> tempModel = m_mapBuilder->GenerateModel(tempMap);
          check(tempModel.IsValid());
          if (!tempModel.IsValid())
          {
            return;
          }

          tempModel->OriginWorldCoord = constructionBP.OriginCoordinate;
          tempModel->MapTileSize = constructionBP.TileSize;

          // マネージャーを更新
          m_map.Reset();
          m_map = ::MoveTemp(tempMap);

          m_model.Reset();
          m_model = ::MoveTemp(tempModel);

          m_mapConstructor->ConstructMap(world, m_map->GetMap(), constructionBP);

          m_mapOperator->AssignMap(m_model);
          m_deployDirector->AssignMap(m_model);
          m_mapObserver->AssignMap(m_model);
          m_eventDirector->AssignMap(m_model);

          discardEvents();
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

          for(const auto& [mapCoord, mapObj] : deployMapObjs)
          {
            m_deployDirector->DeployMapObj(mapObj, mapCoord.x, mapCoord.y);
          }
        }
        void InitNextLevelEvent(ISmithMapEvent* nextLevelEvent)
        {
          // TODO test event
          check((m_eventDirector.IsValid() && m_deployDirector.IsValid()));
          if (!m_eventDirector.IsValid() || !m_deployDirector.IsValid())
          {
            return;
          }

          if (!IS_UINTERFACE_VALID(nextLevelEvent))
          {
            return;
          }

          uint8 nextLevelEventCoordX = 0u;
          uint8 nextLevelEventCoordY = 0u;

          m_eventDirector->DirectNextLevelEventCoord(nextLevelEventCoordX, nextLevelEventCoordY);
          DeployEvent(nextLevelEvent, nextLevelEventCoordX, nextLevelEventCoordY);
        }
        void InitPickableEvent(EMapDeployRule rule, const TArray<ISmithMapEvent*>& events)
        {
          int32 idx = 0;
          uint8 x = 0u;
          uint8 y = 0u;
          while (idx < events.Num())
          {
            if (m_eventDirector->GetDeployableCoord(rule, x, y))
            {
              DeployEvent(events[idx], x, y);
            }

            ++idx;
          }
        }
        void InitDecoration(UWorld* world, const FSmithMapDecoration& decoration)
        {
          TArray<AActor*> decorationActors;
          int32 generateCount = 0;
          switch (decoration.DeployRule)
          {
            case EMapDeployRule::Corner:
            {
              generateCount = FSmithMapHelperLibrary::GetRoomCount(m_map.Get());
            }
            break;

            default:
            {
              unimplemented();
            }
            break;
          }
          m_mapConstructor->ConstructDecoration(world, m_map.Get(), decoration.MapDecoration, generateCount, decorationActors);
          TArray<FMapCoord> deployMapCoords;
          FSmithMapHelperLibrary::GetMapCoordByRule_PerRoom(m_map.Get(), decoration.DeployRule, deployMapCoords);
          check(deployMapCoords.Num() == generateCount);
          check(decorationActors.Num() == generateCount);

          FUECollectionsLibrary::Shuffle(deployMapCoords);

          // TODO need decorationDirector
          for (int32 i = 0; i < generateCount; ++i)
          {    
            if (::IsValid(decorationActors[i]))
            {
              FRotator decorationRotator;
              const FMapCoord mapCoord = deployMapCoords[i];
              FSmithMapHelperLibrary::DirectMapElementRotation(m_map.Get(), decorationRotator, mapCoord.x, mapCoord.y);

              // 壁に付けるように修正
              FMapCoord diffMapCoord;
              if (FSmithMapHelperLibrary::IsSameTileNearby(m_map.Get(), mapCoord, diffMapCoord))
              {
                decorationActors[i]->Destroy();
                continue;
              }
              else
              {
                EDirection direction = FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(diffMapCoord, mapCoord);
                if (direction != EDirection::Invalid)
                {
                  const double yawOfDirection = StaticCast<double>(direction) * 360.0 / StaticCast<double>(EDirection::DirectionCount);
                  decorationRotator.Yaw = yawOfDirection;
                }
              }
              const FVector decorationWorldCoord = FVector
                                                          {
                                                            m_model->OriginWorldCoord.X + StaticCast<double>(mapCoord.x) * StaticCast<double>(m_model->MapTileSize),      
                                                            m_model->OriginWorldCoord.Y + StaticCast<double>(mapCoord.y) * StaticCast<double>(m_model->MapTileSize),      
                                                            m_model->OriginWorldCoord.Z    
                                                          };
              decorationActors[i]->SetActorLocationAndRotation(decorationWorldCoord, decorationRotator);
            }
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
        void FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const FSmithCommandFormat& format)
        {
          m_mapOperator->FindAttackableMapObjs(outAttackableHandles, mapObj, format);
        }
        void FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
        {
          m_mapOperator->FindAttackableMapObjsFromCoord(outAttackableHandles, mapObj, format, offsetToLeft, offsetToTop);
        }
        bool GetPlayerDirection(EDirection& outDirection, ICanSetOnMap* origin, uint8 offsetLeft, uint8 offsetTop)
        {
          // TODO
          uint8 x = 0;
          uint8 y = 0;
          if (!GetMapObjectCoord(origin, x, y))
          {
            return false;
          }
          FMapCoord playerCoord;
          m_mapObserver->GetPlayerCoord(playerCoord);
          outDirection = FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(FMapCoord(x + offsetLeft, y + offsetTop), playerCoord);

          return outDirection != EDirection::Invalid;
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
        bool ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y)
        {
          return m_mapObserver->ConvertMapCoordToWorldLocation(outLocation, x, y);
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
        void discardEvents()
        {
          for (auto& event : m_mapEvents)
          {
            if (IS_UINTERFACE_VALID(event))
            {
              event->DiscardEvent();
            }
          }

          m_mapEvents.Reset();
        }
      private:
        TUniquePtr<FSmithMapBuilder> m_mapBuilder;
        TUniquePtr<FSmithMapConstructor> m_mapConstructor;
        TUniquePtr<FSmithMapObjOperator> m_mapOperator;
        TUniquePtr<FSmithMapDeployDirector> m_deployDirector;
        TUniquePtr<FSmithMapEventDirector> m_eventDirector;
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
    void FSmithMapManager::InitNextLevelEvent(ISmithMapEvent* nextLevelEvent)
    {
      m_pImpl->InitNextLevelEvent(nextLevelEvent);
    }
    void FSmithMapManager::InitPickableEvent(EMapDeployRule rule, const TArray<ISmithMapEvent*>& events)
    {
      m_pImpl->InitPickableEvent(rule, events);
    }
    void FSmithMapManager::InitDecoration(UWorld* world, const FSmithMapDecoration& decoration)
    {
      m_pImpl->InitDecoration(world, decoration);
    }
    void FSmithMapManager::DeployMapObj(ICanSetOnMap* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y);
    }
    void FSmithMapManager::DeployEvent(ISmithMapEvent* mapEvent, uint8 x, uint8 y)
    {
      m_pImpl->DeployEvent(mapEvent, x, y);
    }
    void FSmithMapManager::FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj,const FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outAttackableHandles, mapObj, format);
    } 
    void FSmithMapManager::FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, ICanSetOnMap* mapObj, const FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
    {
      m_pImpl->FindAttackableMapObjsFromCoord(outAttackableHandles, mapObj, format, offsetToLeft, offsetToTop);
    }
    bool FSmithMapManager::GetPlayerDirection(EDirection& outDirection, ICanSetOnMap* origin, uint8 offsetLeft, uint8 offsetTop)
    {
      return m_pImpl->GetPlayerDirection(outDirection, origin, offsetLeft, offsetTop);
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
    bool FSmithMapManager::ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y)
    {
      return m_pImpl->ConvertMapCoordToWorldLocation(outLocation, x, y);
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
