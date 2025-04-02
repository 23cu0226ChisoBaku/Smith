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
#include "SmithMapSystem.h"
#include "SmithRect.h"
#include "SmithMap.h"

#include "ISmithMapModelRequester.h"
#include "SmithMapModelDefinition.h"

#include "MapCoord.h"
#include "TileType.h"
#include "MapObjType.h"
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
#include "IMinimapDisplayer.h"
#include "IMinimapDisplayable.h"
#include "AttackableInfoHandle.h"
#include "ItemGenerationListRow.h"
#include "SmithModelHelperFunctionLibrary.h"
#include "SmithMapDecoration.h"
#include "SmithMapHelperLibrary.h"
#include "UObject/WeakInterfacePtr.h"
#include "DataStructure/Dimension2ArrayHandle.h"
#include "MinimapDisplayTypeFactory.h"
#include "MinimapDisplayType.h"

#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    using namespace UE::Smith::Battle;

    ///
    /// @brief マップシステム実装クラス
    ///
    class FSmithMapSystem::MapSysImpl
    {
      using Model = typename FSmithMapDataModel;
      public:
        //---------------------------------------
        /*
                        ctorとdtor
        */
        //---------------------------------------
        MapSysImpl()
          : m_mapBuilder(::MakeUnique<FSmithMapBuilder>())
          , m_mapConstructor(::MakeUnique<FSmithMapConstructor>())
          , m_mapOperator(::MakeUnique<FSmithMapObjOperator>())
          , m_deployDirector(::MakeUnique<FSmithMapDeployDirector>())
          , m_eventDirector(::MakeUnique<FSmithMapEventDirector>())
          , m_mapObserver(::MakeUnique<FSmithMapObserver>())
          , m_model(::MakeShared<Model>())
          , m_map(nullptr)
          , m_mapEvents{}
          , m_minimapDisplayer(nullptr)
        { }
        ~MapSysImpl()
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

          m_minimapDisplayer = nullptr;
        }

        ///
        /// @brief  イベント登録インターフェースを注入
        ///
        void AssignEventRegister(IEventRegister* eventRegister)
        {
          check(m_mapOperator.IsValid())
          if (!m_mapOperator.IsValid())
          {
            return;
          }

          m_mapOperator->AssignEventRegister(eventRegister);
        }

        ///
        /// @brief  マップモデル取得インターフェースを注入
        ///
        void AssignMapModelRequester(ISmithMapModelRequester* ModelRequester)
        {
          check(ModelRequester != nullptr);
          check(m_model.IsValid());

          m_model->MapModelRequester = ModelRequester;
        }

        ///
        /// @brief  マップを初期化する
        ///
        void InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
        {    
          // 入力チェック
          check(world != nullptr);
          if (world == nullptr)
          {
            return;
          }

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

          // TODO 再代入
          {
            auto requester = m_model->MapModelRequester.Get();
  
            m_model.Reset();
            m_model = ::MoveTemp(tempModel);
            m_model->MapModelRequester = requester;
          }

          m_mapConstructor->ConstructMap(world, m_map->GetMap(), constructionBP);

          m_mapOperator->AssignMapDataModel(m_model);
          m_deployDirector->AssignMapDataModel(m_model);
          m_mapObserver->AssignMapDataModel(m_model);
          m_eventDirector->AssignMapDataModel(m_model);

          discardEvents();

          // マップデータにタイルアクターを設定
          const TArray<AActor*> mapMats = m_mapConstructor->GetMapMaterials();
          for (int32 i = 0; i < mapMats.Num(); ++i)
          {
            AActor* matActor = mapMats[i];
            if (!::IsValid(matActor))
            {
              continue;
            }

            const FVector matLocation = matActor->GetActorLocation();
            const uint8 coordX = StaticCast<uint8>(FMath::RoundHalfToEven(matLocation.X / StaticCast<double>(m_model->MapTileSize)));
            const uint8 coordY = StaticCast<uint8>(FMath::RoundHalfToEven(matLocation.Y / StaticCast<double>(m_model->MapTileSize)));
            
            const FMapCoord matCoord(coordX, coordY);

            if (m_model->ObstacleTable.Contains(matCoord))
            {
              m_model->ObstacleTable[matCoord]->SetTileActor(matActor);
            }
            else if(m_model->StaySpaceTable.Contains(matCoord))
            {
              m_model->StaySpaceTable[matCoord]->SetTileActor(matActor);
            }
          }

          // ミニマップ初期化
          if (m_minimapDisplayer.IsValid())
          {
            m_minimapDisplayer->ResetMap();
            const auto& mapRect = m_map->GetMap();
            const int32 mapTotalRow = StaticCast<int32>(mapRect.GetHeight());
            const int32 mapTotalColumn = StaticCast<int32>(mapRect.GetWidth());
            
            if (m_factory.IsValid())
            {
              TArray<UObject*> alignedMapMats;
              const int32 mapMatsCount = mapTotalRow * mapTotalColumn;
              for (int32 i = 0; i < mapMatsCount; ++i)
              {
                const uint8 curtRow = StaticCast<uint8>(i / mapTotalColumn);
                const uint8 curtColumn = StaticCast<uint8>(i % mapTotalColumn);

                const uint8 mapData = mapRect.GetRect(curtColumn, curtRow);
                IMinimapDisplayable* displayable = nullptr;
                switch (mapData)
                {
                  case (uint8)ETileType::Ground:
                  case (uint8)ETileType::Corridor:
                  {
                    displayable = m_factory->GetDisplayable(EMinimapDisplayType::Ground);
                  }
                  break;
                  case (uint8)ETileType::Wall:
                  {
                    displayable = m_factory->GetDisplayable(EMinimapDisplayType::Wall);
                  }
                  break;
                }
                
                if (displayable == nullptr)
                {
                  alignedMapMats.Emplace(nullptr);
                }
                else
                {
                  alignedMapMats.Emplace(displayable->_getUObject());
                }
              }

              m_minimapDisplayer->AssignMapData(alignedMapMats, mapTotalRow, mapTotalColumn);
            }

          }
        }

        ///
        /// @brief  マップオブジェクトを生成する
        ///
        void InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
        {
          // 入力チェック
          check((world != nullptr) && (player != nullptr));
          check((m_mapObserver.IsValid() && m_deployDirector.IsValid()));
          if (!m_model->MapModelRequester.IsValid())
          {
            return;
          }

          // ダンジョンオブジェクトデータを初期化
          TMap<FMapCoord, AActor*> deployMapObjs{};
          m_mapObserver->InitMapObj(deployMapObjs, world, player, generateBP);

          // オブジェクトをダンジョンに配置
          for(const auto& [mapCoord, mapObj] : deployMapObjs)
          {
            m_deployDirector->DeployMapObj(mapObj, mapCoord.x, mapCoord.y);
          }

          // ミニマップ更新
          if (m_minimapDisplayer.IsValid() && m_factory.IsValid())
          {
            for(const auto& [mapCoord, mapObj] : deployMapObjs)
            {
              const FSmithMapModel mapObjModel = m_model->MapModelRequester->GetModel(mapObj);
              if (!mapObjModel.IsValid())
              {
                continue;
              }

              const uint8 sizeX = mapObjModel.GetSizeX();
              const uint8 sizeY = mapObjModel.GetSizeY();
              const int32 typeObjCnt = StaticCast<int32>(sizeX) * StaticCast<int32>(sizeY);

              const EMapModelType type = mapObjModel.GetModelType();
              TArray<UObject*> tileTypeObjs;
              EMinimapDisplayType displayType = EMinimapDisplayType::Default;

              using enum EMapModelType;
              switch (type)
              {
                case Player:
                {
                  displayType = EMinimapDisplayType::Player;
                }
                break;
                case Enemy:
                {
                  displayType = EMinimapDisplayType::Enemy;
                }
                break;
              }

              for (int32 i = 0; i < typeObjCnt; ++i)
              {
                IMinimapDisplayable* displayable = m_factory->GetDisplayable(displayType);
                if (displayable != nullptr)
                {
                  tileTypeObjs.Emplace(displayable->_getUObject());
                }
              }

              m_minimapDisplayer->ReplaceTiles(tileTypeObjs, mapCoord, sizeX, sizeY);
            }
          }
        }

        ///
        /// @brief  次の階層にいくイベントを初期化
        ///
        void InitNextLevelEvent(ISmithMapEvent* nextLevelEvent)
        {
          check(nextLevelEvent != nullptr);
          if (!m_eventDirector.IsValid() || !m_deployDirector.IsValid())
          {
            return;
          }

          uint8 nextLevelEventCoordX = 0u;
          uint8 nextLevelEventCoordY = 0u;

          // イベントを配置する座標を取得
          m_eventDirector->DirectNextLevelEventCoord(nextLevelEventCoordX, nextLevelEventCoordY);
          // イベントを配置する
          DeployEvent(nextLevelEvent, nextLevelEventCoordX, nextLevelEventCoordY);
          
          // ミニマップ更新
          if (m_minimapDisplayer.IsValid() && m_factory.IsValid())
          {
            TArray<UObject*> tileTypeObjects;
            tileTypeObjects.Emplace(nextLevelEvent->_getUObject());
            m_minimapDisplayer->ReplaceTiles(tileTypeObjects, FMapCoord(nextLevelEventCoordX, nextLevelEventCoordY), 1u, 1u);
          }
        }

        ///
        /// @brief  拾いものイベントを初期化
        ///
        void InitPickableEvent(EMapDeployRule rule, const TArray<ISmithMapEvent*>& events)
        {
          int32 idx = 0;
          uint8 outX = 0u;
          uint8 outY = 0u;
          while (idx < events.Num())
          {
            // ルールでイベント配置できる座標を取得
            if (m_eventDirector->GetDeployableCoord(rule, outX, outY))
            {
              DeployEvent(events[idx], outX, outY);

              // ミニマップ更新
              if (m_minimapDisplayer.IsValid() && m_factory.IsValid())
              {
                TArray<UObject*> tileTypeObjects;
                tileTypeObjects.Emplace(events[idx]->_getUObject());
                m_minimapDisplayer->ReplaceTiles(tileTypeObjects, FMapCoord(outX, outY), 1u, 1u);
              }
            }

            ++idx;
          }       
        }

        ///
        /// @brief  ダンジョン飾りを配置する
        ///
        void InitDecoration(UWorld* world, const FSmithMapDecoration& decoration)
        {
          TArray<AActor*> decorationActors;
          int32 generateCount = 0;

          using enum EMapDeployRule;
          switch (decoration.DeployRule)
          {
            case Corner:
            case Corner_Per_Room:
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

          // 飾りを作る
          m_mapConstructor->ConstructDecoration(world, m_map.Get(), decoration.MapDecoration, generateCount, decorationActors);
          TArray<FMapCoord> deployMapCoords;
          FSmithMapHelperLibrary::GetMapCoordsByRule(m_map.Get(), decoration.DeployRule, deployMapCoords);
          
          check(deployMapCoords.Num() == generateCount);
          check(decorationActors.Num() == generateCount);

          FUECollectionsLibrary::Shuffle(deployMapCoords);

          // 飾りをマップに配置する
          for (int32 i = 0; i < generateCount; ++i)
          {    
            if (decorationActors[i] != nullptr)
            {
              FRotator decorationRotator;
              const FMapCoord mapCoord = deployMapCoords[i];
              FSmithMapHelperLibrary::DirectMapElementRotation(m_map.Get(), decorationRotator, mapCoord.x, mapCoord.y);

              // TODO 壁に付けるように修正
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

        ///
        /// @brief  ミニマップディスプレイヤーを注入
        ///
        void AssignMinimapDisplayer(IMinimapDisplayer* minimapDisplayer)
        {
          check(minimapDisplayer != nullptr);

          m_minimapDisplayer = minimapDisplayer;

          // Bind Minimap Update Event
          TDelegate<void(UObject*, const FMapCoord&, uint8, uint8)> minimapUpdateEventDelegate;

          // ミニマップ更新ラムダ式
          auto minimapUpdateLambda = [this](UObject* displayable, const FMapCoord& coord, uint8 sizeX, uint8 sizeY) 
          {
            if (!m_minimapDisplayer.IsValid())
            {
              return;
            }
            
            m_minimapDisplayer->UpdateMinimap(displayable, coord, sizeX, sizeY);
          };
          minimapUpdateEventDelegate.BindLambda(minimapUpdateLambda);

          m_deployDirector->AddUpdateMinimapDelegate(minimapUpdateEventDelegate);
          m_mapOperator->AddUpdateMinimapDelegate(minimapUpdateEventDelegate);

        }

        ///
        /// @brief  ミニマップオブジェクトファクトリーを注入
        ///
        void AssignMinimapDisplayTypeFactory(UMinimapDisplayTypeFactory* factory)
        {
          m_factory = factory;
        }

        /// 
        /// @brief  マップオブジェクトを配置する
        ///
        void DeployMapObj(AActor* mapObj, uint8 x, uint8 y)
        {
          m_deployDirector->DeployMapObj(mapObj, x, y);
        }

        /// 
        /// @brief  マップイベントを配置する
        ///
        void DeployEvent(ISmithMapEvent* mapEvent, uint8 x, uint8 y)
        {
          m_deployDirector->DeployEvent(mapEvent, x, y);
          
          if (IS_UINTERFACE_VALID(mapEvent))
          {
            m_mapEvents.Emplace(mapEvent);
          }
        }

        ///
        /// @brief  攻撃できるオブジェクトを探す                      
        ///
        void FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor* mapObj, const FSmithCommandFormat& format)
        {
          m_mapOperator->FindAttackableMapObjs(outAttackableHandles, mapObj, format);
        }

        ///
        /// @brief  攻撃できるオブジェクトを特定の座標から探す                      
        ///
        void FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor* mapObj, const FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
        {
          m_mapOperator->FindAttackableMapObjsFromCoord(outAttackableHandles, mapObj, format, offsetToLeft, offsetToTop);
        }

        ///
        /// @brief  プレイヤーと請求者の方向関係を取得
        ///
        bool GetPlayerDirection(EDirection& outDirection, EDirectionPolicy directionStrategy, AActor* origin, uint8 offsetLeft, uint8 offsetTop)
        {
          uint8 x = 0;
          uint8 y = 0;
          if (!GetMapObjectCoord(origin, x, y))
          {
            return false;
          }
          FMapCoord playerCoord;
          m_mapObserver->GetPlayerCoord(playerCoord);
          outDirection = FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(FMapCoord(x + offsetLeft, y + offsetTop), playerCoord, directionStrategy);

          return outDirection != EDirection::Invalid;
        }

        ///
        /// @brief  マップオブジェクトを移動
        ///
        void MoveMapObj(AActor* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
        {
          m_mapOperator->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
        }

        ///
        /// @brief  プレイヤーを追跡する
        ///
        bool ChasePlayerTarget(EDirection& outChaseDirection, AActor* chaser, uint8 chaseRadius)
        {
          return m_mapObserver->ChasePlayer(outChaseDirection, chaser, chaseRadius);
        }

        ///
        /// @brief  オブジェクトの座標を取得 
        ///
        bool GetMapObjectCoord(AActor* mapObj, uint8& x, uint8& y)
        {
          return m_mapObserver->GetMapObjectCoord(mapObj, x, y);
        }

        ///
        /// @brief  ダンジョン座標をワールドに変換
        ///
        bool ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y)
        {
          return m_mapObserver->ConvertMapCoordToWorldLocation(outLocation, x, y);
        }

        ///
        /// @brief  マップシステムをリセット
        ///
        void Reset()
        {
          m_mapObserver->ClearMapObjs_IgnorePlayer();
          m_mapConstructor->DestructMap();
        }

        /// @brief            GCに解放されたくないオブジェクトをcollectorに登録
        /// @param collector 
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

        /// @brief  イベントを解除
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
        TWeakInterfacePtr<IMinimapDisplayer> m_minimapDisplayer;
        TWeakObjectPtr<UMinimapDisplayTypeFactory> m_factory;
    };

    #pragma region FSmithMapSystem Interface
    FSmithMapSystem::FSmithMapSystem()
      : m_pImpl(::MakeUnique<MapSysImpl>())
    { }

    FSmithMapSystem::~FSmithMapSystem()
    {
      m_pImpl.Reset();
    }

    void FSmithMapSystem::AssignEventRegister(IEventRegister* eventRegister)
    {
      m_pImpl->AssignEventRegister(eventRegister);
    }

    void FSmithMapSystem::AssignMapModelRequester(ISmithMapModelRequester* MapModelRequester)
    {
      m_pImpl->AssignMapModelRequester(MapModelRequester);
    }

    void FSmithMapSystem::InitMap(UWorld* world, const FSmithMapBluePrint& mapBP, const FSmithMapConstructionBluePrint& constructionBP)
    {
      m_pImpl->InitMap(world, mapBP, constructionBP);
    }

    void FSmithMapSystem::InitMapObjs(UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
    {
      m_pImpl->InitMapObjs(world, player, generateBP);
    }

    void FSmithMapSystem::InitNextLevelEvent(ISmithMapEvent* nextLevelEvent)
    {
      m_pImpl->InitNextLevelEvent(nextLevelEvent);
    }

    void FSmithMapSystem::InitPickableEvent(EMapDeployRule rule, const TArray<ISmithMapEvent*>& events)
    {
      m_pImpl->InitPickableEvent(rule, events);
    }

    void FSmithMapSystem::InitDecoration(UWorld* world, const FSmithMapDecoration& decoration)
    {
      m_pImpl->InitDecoration(world, decoration);
    }

    void FSmithMapSystem::AssignMinimapDisplayer(IMinimapDisplayer* minimapDisplayer)
    {
      m_pImpl->AssignMinimapDisplayer(minimapDisplayer);
    }

    void FSmithMapSystem::AssignMinimapDisplayTypeFactory(UMinimapDisplayTypeFactory* factory)
    {
      m_pImpl->AssignMinimapDisplayTypeFactory(factory);
    }

    void FSmithMapSystem::DeployMapObj(AActor* mapObj, uint8 x, uint8 y)
    {
      m_pImpl->DeployMapObj(mapObj, x, y);
    }

    void FSmithMapSystem::DeployEvent(ISmithMapEvent* mapEvent, uint8 x, uint8 y)
    {
      m_pImpl->DeployEvent(mapEvent, x, y);
    }

    void FSmithMapSystem::FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor* mapObj,const FSmithCommandFormat& format)
    {
      m_pImpl->FindAttackableMapObjs(outAttackableHandles, mapObj, format);
    } 

    void FSmithMapSystem::FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor* mapObj, const FSmithCommandFormat& format, uint8 offsetToLeft, uint8 offsetToTop)
    {
      m_pImpl->FindAttackableMapObjsFromCoord(outAttackableHandles, mapObj, format, offsetToLeft, offsetToTop);
    }

    bool FSmithMapSystem::GetPlayerDirection(EDirection& outDirection, EDirectionPolicy directionStrategy, AActor* origin, uint8 offsetLeft, uint8 offsetTop)
    {
      return m_pImpl->GetPlayerDirection(outDirection, directionStrategy, origin, offsetLeft, offsetTop);
    }

    void FSmithMapSystem::MoveMapObj(AActor* mapObj, EDirection moveDirection, uint8 moveDistance, FVector& destination)
    {
      return m_pImpl->MoveMapObj(mapObj, moveDirection, moveDistance, destination);
    }

    bool FSmithMapSystem::ChasePlayerTarget(EDirection& outChaseDirection, AActor* chaser, uint8 chaseRadius)
    {
      return m_pImpl->ChasePlayerTarget(outChaseDirection, chaser, chaseRadius);
    }

    bool FSmithMapSystem::GetMapObjectCoord(AActor* mapObj, uint8& x, uint8& y)
    {
      return m_pImpl->GetMapObjectCoord(mapObj, x, y);
    }

    bool FSmithMapSystem::ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y)
    {
      return m_pImpl->ConvertMapCoordToWorldLocation(outLocation, x, y);
    }

    void FSmithMapSystem::Reset()
    {
      m_pImpl->Reset();
    }

    void FSmithMapSystem::AddReferencedObjects(FReferenceCollector& collector)
    {
      m_pImpl->AddReferencedObjects(collector);
    }

    FString FSmithMapSystem::GetReferencerName() const
    {
      return TEXT("Smith Map Manager");
    }

    #pragma endregion FSmithMapSystem Interface
  }
}
