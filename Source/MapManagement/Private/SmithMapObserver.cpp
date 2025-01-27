// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObserver.cpp

Author : MAI ZHICONG

Description : マップを観察し処理するクラス(※Smithマップ専用)

Update History: 2025/01/07 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapObserver.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "MapCoord.h"
#include "ICanSetOnMap.h"
#include "SmithMapDataModel.h"
#include "SmithSection.h"
#include "Direction.h"
#include "SmithMapHelperFunc.h"
#include "MLibrary.h"

// 内部使用(シャッフル)
namespace UE::Smith::Map::Private
{
  template<typename T>
  void RandomShuffle(TArray<T>& arr)
  {
    const int32 arrLastIndex = arr.Num() - 1;
    for (int32 i = 0; i <= arrLastIndex; ++i)
    {
      int32 index = FMath::RandRange(i, arrLastIndex);
      if (i != index)
      {
        arr.Swap(i, index);
      }
    }
  }
}

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithMapObserver実装クラス 
    ///
    class FSmithMapObserver::MapObserverImpl
    {
      using Model = typename FSmithMapDataModel;
      public:
        MapObserverImpl()
        { }
        ~MapObserverImpl()
        { }
      public:
        void AssignMap(TSharedPtr<Model> pModel, FVector originCoord_World, int32 tileSize)
        {
          m_model = pModel;
          m_originCoord_World = originCoord_World;
          m_mapTileSize = tileSize;
        }
        void InitMapObj(TMap<FMapCoord, ICanSetOnMap*>& outMapObjs, UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
        {
          using namespace UE::Smith::Map::Private;

          // AssignMapをこの前に呼び出す必要がある
          check(m_model.IsValid())
          if (!m_model.IsValid()) [[unlikely]]
          {
            return;
          }

          if (!::IsValid(world) && !::IsValid(player))
          {
            return;
          }

          // マップに置けないプレイヤーだと初期化しない
          ICanSetOnMap* playerMapObj = Cast<ICanSetOnMap>(player);
          if (playerMapObj == nullptr)
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return;
          }

          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
          if (!map_shared.IsValid())
          {
            return;
          }

          outMapObjs.Reset();

          const uint8 mapRow = map_shared->GetRow();
          const uint8 mapColumn = map_shared->GetColumn();

          // 空いているタイルの座標を入れる
          TArray<FMapCoord> remainCoords{};

          for (uint8 y = 0; y < mapRow; ++y)
          {
            for (uint8 x = 0; x < mapColumn; ++x)
            {
              FSmithSection* section = map_shared->GetSection(y, x);
              if (section == nullptr)
              {
                continue;
              }

              const uint8 roomWidth = section->GetRoomWidth();
              const uint8 roomHeight = section->GetRoomHeight();
              // 部屋の横幅と縦幅のいずれかが１以下だったらオブジェクトを生成しない
              if (roomWidth <= 1 && roomHeight <= 1)
              {
                continue;
              }

              const uint8 sectionLeft = map_shared->GetSectionLeft(x);
              const uint8 sectionTop = map_shared->GetSectionTop(y);
              const uint8 roomLeft = sectionLeft + section->GetRoomLeft();
              const uint8 roomTop = sectionTop + section->GetRoomTop();

              // 今の部屋のタイル情報を入れるコンテナ
              TArray<FMapCoord> roomCoords{};
              for (uint8 roomY = 0; roomY < roomHeight; ++roomY)
              {
                for (uint8 roomX = 0; roomX < roomWidth; ++roomX)
                {
                  const FMapCoord roomCoord(roomLeft + roomX, roomTop + roomY);
                  roomCoords.Emplace(roomCoord);
                }
              }   

              // マップのタイル数が生成する敵の数より小さいするとき例外として処理
              check(roomCoords.Num() >= generateBP.InitGenerateCountPerRoom);
              RandomShuffle(roomCoords);

              // 敵のBPクラスを取得
              TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(*generateBP.EnemyBPPath)).LoadSynchronous();

              if (subClass == nullptr)
              {
                MDebug::LogError("Invalid Enemy BP Path");
                return;
              }

              for (int32 i = 0; i < StaticCast<int32>(generateBP.InitGenerateCountPerRoom); ++i)
              {
                const FMapCoord mapCoord = roomCoords[i]; 
                // タイルがモデルに入ってないと処理を中断する
                if (!model_shared->StaySpaceTable.Contains(mapCoord))
                {
                  return;
                }

                const FVector spawnWorldCoord = FVector(
                                                         m_originCoord_World.X + StaticCast<double>(StaticCast<int32>(mapCoord.x) * m_mapTileSize),
                                                         m_originCoord_World.Y + StaticCast<double>(StaticCast<int32>(mapCoord.y) * m_mapTileSize),
                                                         m_originCoord_World.Z
                                                        );
                AActor* enemy = world->SpawnActor<AActor>(subClass, spawnWorldCoord, FRotator::ZeroRotator);

                // 生成した敵がマップに置けないと処理を中断する
                ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(enemy);
                if (mapObj == nullptr)
                {
                  MDebug::LogError("Can not place actor on map because it is not implemented ICanSetOnMap");
                  enemy->Destroy();
                  return;
                }

                outMapObjs.Emplace(mapCoord, mapObj);

              }

              // 何も入っていない部屋のタイルの座標を入れて、プレイヤー生成するとき使う
              for (int32 i = StaticCast<int32>(generateBP.InitGenerateCountPerRoom); i < roomCoords.Num(); ++ i)
              {
                remainCoords.Emplace(roomCoords[i]);
              }
            }
          }

          check(remainCoords.Num() > 0);
          RandomShuffle(remainCoords);
          
          // プレイヤーを配置
          const FMapCoord playerMapCoord = remainCoords[0];
          const FVector playerWorldCoord = FVector(
                                                    m_originCoord_World.X + StaticCast<double>(StaticCast<int32>(playerMapCoord.x) * m_mapTileSize),
                                                    m_originCoord_World.Y + StaticCast<double>(StaticCast<int32>(playerMapCoord.y) * m_mapTileSize),
                                                    m_originCoord_World.Z
                                                  ); 
          player->SetActorLocation(playerWorldCoord);

          outMapObjs.Emplace(playerMapCoord, playerMapObj);

          m_player = playerMapObj;
          m_generateBP = generateBP;
        }
        void ClearMapObjs_IgnorePlayer()
        {
          check(m_model.IsValid())
          if (!m_model.IsValid())
          {
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          for (const auto& onMapObj : model_shared->OnMapObjsCoordTable)
          {
            if (!onMapObj.Key.IsValid() || onMapObj.Key.Get() == m_player.Get())
            {
              continue;
            }

            AActor* objActor = Cast<AActor>(onMapObj.Key.GetObject());
            if (objActor != nullptr)
            {
              objActor->Destroy();
            }
          }
        }
        void InitNextLevelEvent_Temp(uint8& outX, uint8& outY, FVector& destination)
        {
          using namespace UE::Smith::Map::Private;

          check (m_model.IsValid())
          if (!m_model.IsValid())
          {
            MDebug::LogError("failed to InitNextLevelEvent --- model invalid");
            return;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          check(model_shared->Map.IsValid())
          if (!model_shared->Map.IsValid())
          {
            MDebug::LogError("failed to InitNextLevelEvent --- map invalid");
            return;
          }
          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
          TArray<uint8> sectionIdx{};
          sectionIdx.Reserve(map_shared->GetSectionCount());

          check(map_shared->GetSectionCount() > 0)
          if (map_shared->GetSectionCount() == 0)
          {
            MDebug::LogError("failed to InitNextLevelEvent --- map section num invalid");
            return;
          }

          outX = 0u;
          outY = 0u;
          destination = FVector::ZeroVector;

          for (uint8 i = 0u; i < map_shared->GetSectionCount(); ++i)
          {
            sectionIdx.Emplace(i);
          }

          RandomShuffle(sectionIdx);
          for(int32 i = 0; i < sectionIdx.Num(); ++i)
          {
            const uint8 sectionRow = sectionIdx[i] / map_shared->GetColumn();
            const uint8 sectionColumn = sectionIdx[i] % map_shared->GetColumn();

            FSmithSection* sectionPtr = map_shared->GetSection(sectionRow, sectionColumn);

            if (sectionPtr == nullptr || !sectionPtr->HasRoom())
            {
              continue;
            }

            if (sectionPtr->GetRoomWidth() <= 2 && sectionPtr->GetRoomHeight() <= 2)
            {
              continue;
            }

            const uint8 sectionRoomLeft = map_shared->GetSectionLeft(sectionColumn) + sectionPtr->GetRoomLeft();
            const uint8 sectionRoomRight = sectionRoomLeft + sectionPtr->GetRoomWidth() - 1;
            const uint8 sectionRoomTop = map_shared->GetSectionTop(sectionRow) + sectionPtr->GetRoomTop();
            const uint8 sectionRoomBottom = sectionRoomTop + sectionPtr->GetRoomHeight() - 1;

            TArray<FMapCoord> eventRandomPlaceCoords{};
            // 四隅に階段を配置しない
            eventRandomPlaceCoords.Reserve(StaticCast<int32>(sectionPtr->GetRoomWidth()) * 2 + StaticCast<int32>(sectionPtr->GetRoomHeight()) * 2 - 8);
            // 部屋の上と下両辺の座標
            for (uint8 column = sectionRoomLeft + 1u; column < sectionRoomRight; ++column)
            {
              eventRandomPlaceCoords.Emplace(FMapCoord{column, sectionRoomTop});
              eventRandomPlaceCoords.Emplace(FMapCoord{column, sectionRoomBottom});
            }

            for (uint8 row = sectionRoomTop + 1u; row < sectionRoomBottom; ++row)
            {
              eventRandomPlaceCoords.Emplace(FMapCoord{sectionRoomLeft, row});
              eventRandomPlaceCoords.Emplace(FMapCoord{sectionRoomRight, row});
            }

            RandomShuffle(eventRandomPlaceCoords);
            bool success = false;
            for (int32 j = 0; j < eventRandomPlaceCoords.Num(); ++j)
            {
              const FMapCoord mapCoord = eventRandomPlaceCoords[j];

              if (!model_shared->StaySpaceTable.Contains(mapCoord))
              {
                continue;
              }

            if (canPlaceEvent(mapCoord))
              {
                success = true;
                outX = mapCoord.x;
                outY = mapCoord.y;
                break;
              }
            
            }

            if (success)
            {
              destination = FVector{
                                      StaticCast<double>(outX) * StaticCast<double>(m_mapTileSize) + m_originCoord_World.X,
                                      StaticCast<double>(outY) * StaticCast<double>(m_mapTileSize) + m_originCoord_World.Y,
                                      m_originCoord_World.Z,
                                    };
              break;
            }
          }
        }
        bool GetMapObjectCoord(ICanSetOnMap* mapObj, uint8& outX, uint8& outY)
        {
          outX = 0;
          outY = 0;

          check(m_model.IsValid())
          if (!m_model.IsValid())
          {
            return false;
          }

          if (!IS_UINTERFACE_VALID(mapObj))
          {
            return false;
          }

          TQueue<int32> a;
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared->OnMapObjsCoordTable.Contains(mapObj))
          {
            MDebug::LogError("Can not get Coord --- invalid mapObj");
            return false;
          }

          const FMapCoord mapCoord = model_shared->OnMapObjsCoordTable[mapObj];
          outX = mapCoord.x;
          outY = mapCoord.y;
          return true;
        }
        bool ChasePlayer(EDirection& outChaseDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
        {
          return ChaseTarget(outChaseDirection, chaser, m_player.Get(), chaseRadius);
        }
        bool ChaseTarget(EDirection& outChaseDirection, ICanSetOnMap* chaser, ICanSetOnMap* target, uint8 chaseRadius)
        {
          outChaseDirection = EDirection::Invalid;

          check(m_model.IsValid())
          if (!m_model.IsValid())
          {
            return false;
          }
          // TODO
          // Chase Player Use Only
          if (!m_player.IsValid() || m_player != target)
          {
            return false;
          }

          if (!IS_UINTERFACE_VALID(chaser) || !IS_UINTERFACE_VALID(target) || chaseRadius == 0)
          {
            return false;
          }

          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return false;
          }

          if (!model_shared->OnMapObjsCoordTable.Contains(chaser)
              || !model_shared->OnMapObjsCoordTable.Contains(target))
          {
            return false; 
          }

          for (uint8 curtRadius = 1u; curtRadius <= chaseRadius; ++curtRadius)
          {
            chaseTarget_internal(outChaseDirection, chaser, target, curtRadius);
            if (outChaseDirection != EDirection::Invalid)
            {
              return true;
            }
          }

          return false;
        }

      private:
        bool isInSameSection(ICanSetOnMap* chaser, ICanSetOnMap* target)
        {
          if (!IS_UINTERFACE_VALID(chaser) || !IS_UINTERFACE_VALID(target)) [[unlikely]]
          {
            return false;
          }
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared->OnMapObjsCoordTable.Contains(chaser)
              || !model_shared->OnMapObjsCoordTable.Contains(target)) [[unlikely]]
          {
            return false; 
          }

          if (!model_shared->Map.IsValid())
          {
            MDebug::LogError("Invalid Map --- SmithMapObserver->isInSameRoom");
            return false;
          }
          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();

          const uint8 chaserCoordX = model_shared->OnMapObjsCoordTable[chaser].x;
          const uint8 chaserCoordY = model_shared->OnMapObjsCoordTable[chaser].y;
          const uint8 targetCoordX = model_shared->OnMapObjsCoordTable[target].x;
          const uint8 targetCoordY = model_shared->OnMapObjsCoordTable[target].y;

          return FSmithMapHelperFunc::IsInSameSection(map_shared.Get(), chaserCoordX, chaserCoordY, targetCoordX, targetCoordY);
        }
        void chaseTarget_internal(EDirection& outChaseDirection, ICanSetOnMap* chaser, ICanSetOnMap* target, uint8 chaseRadius)
        {
          if (!m_model.IsValid()) [[unlikely]]
          {
            MDebug::LogError("model invalid --- chaseTarget_internal");
            return;
          } 

          TSharedPtr<FSmithMapDataModel> model_shared = m_model.Pin();
          if (!model_shared->Map.IsValid()) [[unlikely]]
          {
            MDebug::LogError("map invalid --- chaseTarget_internal");
            return;
          } 
          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();

          const uint8 chaserCoordX = model_shared->OnMapObjsCoordTable[chaser].x;
          const uint8 chaserCoordY = model_shared->OnMapObjsCoordTable[chaser].y;
          const uint8 mapWidth = map_shared->GetMapWidth();
          const uint8 mapHeight = map_shared->GetMapHeight();

          int32 chaseCoordX_Offset = chaseRadius;
          int32 chaseCoordY_Offset = 0;
          bool bChaseSucceed = false;

          // TODO Need Comment Immediately
          while (chaseCoordX_Offset > 0)
          {
            {
              const int32 checkMapCoordX = StaticCast<int32>(chaserCoordX) + chaseCoordX_Offset;
              const int32 checkMapCoordY = StaticCast<int32>(chaserCoordY) + chaseCoordY_Offset;
              if ((checkMapCoordX >= 0 && checkMapCoordX < StaticCast<int32>(mapWidth))
                  && (checkMapCoordY >= 0 && checkMapCoordY < StaticCast<int32>(mapHeight))
                )
              {
                const FMapCoord checkMapCoord{StaticCast<uint8>(checkMapCoordX), StaticCast<uint8>(checkMapCoordY)};  
                if (model_shared->StaySpaceTable.Contains(checkMapCoord))
                {
                  if (model_shared->StaySpaceTable[checkMapCoord]->GetMapObject() == target)
                  {
                    bChaseSucceed = true;
                    break;
                  }
                }
              }
            }

            {
              const int32 checkMapCoordX_Reverse = StaticCast<int32>(chaserCoordX) - chaseCoordX_Offset;
              const int32 checkMapCoordY_Reverse = StaticCast<int32>(chaserCoordY) - chaseCoordY_Offset;
              
              if ((checkMapCoordX_Reverse >= 0 && checkMapCoordX_Reverse < StaticCast<int32>(mapWidth))
                  && (checkMapCoordY_Reverse >= 0 && checkMapCoordY_Reverse < StaticCast<int32>(mapHeight))
                )
              {
                const FMapCoord checkMapCoord_Reverse{StaticCast<uint8>(checkMapCoordX_Reverse), StaticCast<uint8>(checkMapCoordY_Reverse)};  
                if (model_shared->StaySpaceTable.Contains(checkMapCoord_Reverse))
                {
                  if (model_shared->StaySpaceTable[checkMapCoord_Reverse]->GetMapObject() == target)
                  {
                    chaseCoordX_Offset *= -1;
                    chaseCoordY_Offset *= -1;
                    bChaseSucceed = true;
                    break;
                  }
                }
              }
            }
            --chaseCoordX_Offset;
            ++chaseCoordY_Offset;
          }

          if (!bChaseSucceed)
          {
            while (chaseCoordX_Offset > -StaticCast<int32>(chaseRadius))
            {
              {
                const int32 checkMapCoordX = StaticCast<int32>(chaserCoordX) + chaseCoordX_Offset;
                const int32 checkMapCoordY = StaticCast<int32>(chaserCoordY) + chaseCoordY_Offset;
                if ((checkMapCoordX >= 0 && checkMapCoordX < StaticCast<int32>(mapWidth))
                    && (checkMapCoordY >= 0 && checkMapCoordY < StaticCast<int32>(mapHeight))
                  )
                {
                  const FMapCoord checkMapCoord{StaticCast<uint8>(checkMapCoordX), StaticCast<uint8>(checkMapCoordY)};  
                  if (model_shared->StaySpaceTable.Contains(checkMapCoord))
                  {
                    if (model_shared->StaySpaceTable[checkMapCoord]->GetMapObject() == target)
                    {
                      bChaseSucceed = true;
                      break;
                    }
                  }
                }
              }

              {
                const int32 checkMapCoordX_Reverse = StaticCast<int32>(chaserCoordX) - chaseCoordX_Offset;
                const int32 checkMapCoordY_Reverse = StaticCast<int32>(chaserCoordY) - chaseCoordY_Offset;
                
                if ((checkMapCoordX_Reverse >= 0 && checkMapCoordX_Reverse < StaticCast<int32>(mapWidth))
                    && (checkMapCoordY_Reverse >= 0 && checkMapCoordY_Reverse < StaticCast<int32>(mapHeight))
                  )
                {
                  const FMapCoord checkMapCoord_Reverse{StaticCast<uint8>(checkMapCoordX_Reverse), StaticCast<uint8>(checkMapCoordY_Reverse)};  
                  if (model_shared->StaySpaceTable.Contains(checkMapCoord_Reverse))
                  {
                    if (model_shared->StaySpaceTable[checkMapCoord_Reverse]->GetMapObject() == target)
                    {
                      chaseCoordX_Offset *= -1;
                      chaseCoordY_Offset *= -1;
                      bChaseSucceed = true;
                      break;
                    }
                  }
                }
              }
              --chaseCoordX_Offset;
              --chaseCoordY_Offset;
            }
          }

          if (!bChaseSucceed)
          {
            outChaseDirection = EDirection::Invalid;
            return;
          }

          if (chaseCoordX_Offset > 0)
          {
            if (chaseCoordY_Offset > 0)
            {
              outChaseDirection = EDirection::NorthEast;
            }
            else if (chaseCoordY_Offset < 0)
            {
              outChaseDirection = EDirection::NorthWest;
            }
            else
            {
              outChaseDirection = EDirection::North;
            }
          }
          else if (chaseCoordX_Offset < 0)
          {
            if (chaseCoordY_Offset > 0)
            {
              outChaseDirection = EDirection::SouthEast;
            }
            else if (chaseCoordY_Offset < 0)
            {
              outChaseDirection = EDirection::SouthWest;
            }
            else
            {
              outChaseDirection = EDirection::South;
            }
          }
          else
          {
            if (chaseCoordY_Offset > 0)
            {
              outChaseDirection = EDirection::East;
            }
            else if (chaseCoordY_Offset < 0)
            {
              outChaseDirection = EDirection::West;
            } 
            else
            {
              outChaseDirection = EDirection::Invalid;
            }
          }
        }
        // TODO need check out of bounds;
        bool canPlaceEvent(FMapCoord coord, bool bPlaceOnCrossroad = false)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (model_shared->StaySpaceTable[coord]->GetMapObject() == nullptr 
              && model_shared->StaySpaceTable[coord]->GetEvent() == nullptr
             )
          {
            if (!bPlaceOnCrossroad)
            {
              // 4だったら交差点
              int32 crossCount = 0;
              {
                //const FMapCoord coord_north{coord.x + 1u, coord.y};
                // if (model_shared->StaySpaceTable.Contains(coord_north))
                // {
                //   ++crossCount;
                // }
              }
              {
                //const FMapCoord coord_east{coord.x, coord.y + 1u};
                // if (model_shared->StaySpaceTable.Contains(coord_east))
                // {
                //   ++crossCount;
                // }
              }
              {
                //const FMapCoord coord_south{coord.x - 1u, coord.y};
                // if (model_shared->StaySpaceTable.Contains(coord_south))
                // {
                //   ++crossCount;
                // }
              }
              {
                // const FMapCoord coord_west{coord.x, coord.y - 1u};
                // if (model_shared->StaySpaceTable.Contains(coord_west))
                // {
                //   ++crossCount;
                // }
              }

              if (crossCount == 4)
              {
                return false;
              }
              else
              {
                return true;
              }
            }
            else
            {
              return true;
            }
          }
          else
          {
            return false;
          }
        }
      private:
        TWeakPtr<Model> m_model;
        TWeakInterfacePtr<ICanSetOnMap> m_player;
        FSmithEnemyGenerateBluePrint m_generateBP;
        FVector m_originCoord_World;
        int32 m_mapTileSize;
        uint8 m_turnPastCnt;
    };
    FSmithMapObserver::FSmithMapObserver()
      : m_pImpl(::MakeUnique<MapObserverImpl>())
    { }

    FSmithMapObserver::~FSmithMapObserver()
    {
      m_pImpl.Reset();
    }

    FSmithMapObserver::FSmithMapObserver(FSmithMapObserver&& other) noexcept
      : m_pImpl(::MoveTemp(other.m_pImpl))
    { }

    FSmithMapObserver& FSmithMapObserver::operator=(FSmithMapObserver&& other) noexcept
    {
      if (this != &other)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }
    void FSmithMapObserver::AssignMap(TSharedPtr<FSmithMapDataModel> pModel, FVector originCoord_World, int32 mapTileSize)
    {
      m_pImpl->AssignMap(pModel, originCoord_World, mapTileSize);
    }
    void FSmithMapObserver::InitMapObj(TMap<FMapCoord, ICanSetOnMap*>& outMapObjs, UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
    {
      m_pImpl->InitMapObj(outMapObjs, world, player, generateBP);
    }
    void FSmithMapObserver::ClearMapObjs_IgnorePlayer()
    {
      m_pImpl->ClearMapObjs_IgnorePlayer();
    }
    void FSmithMapObserver::InitNextLevelEvent_Temp(uint8& outX, uint8& outY, FVector& destination)
    {
      m_pImpl->InitNextLevelEvent_Temp(outX, outY, destination);
    }
    bool FSmithMapObserver::ChasePlayer(EDirection& outChaseDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
    {
      return m_pImpl->ChasePlayer(outChaseDirection, chaser, chaseRadius);
    }
    bool FSmithMapObserver::GetMapObjectCoord(ICanSetOnMap* mapObj, uint8& outX, uint8& outY)
    {
      return m_pImpl->GetMapObjectCoord(mapObj, outX, outY);
    }
    void FSmithMapObserver::GenerateNewEnemy()
    {

    }
    
    bool FSmithMapObserver::ChaseTarget(EDirection& outChaseDirection, ICanSetOnMap* chaser, ICanSetOnMap* target, uint8 chaseRadius)
    {
      return m_pImpl->ChaseTarget(outChaseDirection, chaser, target, chaseRadius);
    }

  }
}
