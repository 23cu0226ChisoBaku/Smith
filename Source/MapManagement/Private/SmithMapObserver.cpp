// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapObserver.h"

#include "UObject/WeakInterfacePtr.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "MapCoord.h"
#include "ICanSetOnMap.h"
#include "SmithMapDataModel.h"
#include "SmithSection.h"

#include "MLibrary.h"

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
    class FSmithMapObserver::MapObserverImpl
    {
      public:
        MapObserverImpl()
        { }
        ~MapObserverImpl()
        { }
      public:
        void AssignMap(TSharedPtr<FSmithMapDataModel> pModel, FVector originCoord_World, int32 tileSize)
        {
          m_model = pModel;
          m_originCoord_World = originCoord_World;
          m_mapTileSize = tileSize;
        }
        void InitMapObj(TMap<FMapCoord, ICanSetOnMap*>& outMapObjs, UWorld* world, AActor* player, const FSmithEnemyGenerateBluePrint& generateBP)
        {
          using namespace UE::Smith::Map::Private;

          check(m_model.IsValid())
          if (!m_model.IsValid())
          {
            return;
          }

          if (!::IsValid(world) && !::IsValid(player))
          {
            return;
          }

          ICanSetOnMap* playerMapObj = Cast<ICanSetOnMap>(player);
          if (playerMapObj == nullptr)
          {
            return;
          }

          TSharedPtr<FSmithMapDataModel> model_shared = m_model.Pin();
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

              TArray<FMapCoord> roomCoords{};

              for (uint8 roomY = 0; roomY < roomHeight; ++roomY)
              {
                for (uint8 roomX = 0; roomX < roomWidth; ++roomX)
                {
                  const FMapCoord roomCoord(roomLeft + roomX, roomTop + roomY);
                  roomCoords.Emplace(roomCoord);
                }
              }   

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

                // TODO
                ICanSetOnMap* mapObj = Cast<ICanSetOnMap>(enemy);
                if (mapObj == nullptr)
                {
                  MDebug::LogError("Can not place actor on map because it is not implemented ICanSetOnMap");
                  enemy->Destroy();
                  return;
                }

                outMapObjs.Emplace(mapCoord, mapObj);

              }

              for (int32 i = StaticCast<int32>(generateBP.InitGenerateCountPerRoom); i < roomCoords.Num(); ++ i)
              {
                remainCoords.Emplace(roomCoords[i]);
              }
            }
          }

          check(remainCoords.Num() > 0);
          // プレイヤーを配置
          RandomShuffle(remainCoords);
          
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
      private:
        TWeakPtr<FSmithMapDataModel> m_model;
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
    void FSmithMapObserver::GenerateNewEnemy()
    {

    }
    void FSmithMapObserver::ChaseTarget(ICanSetOnMap* chaser, ICanSetOnMap* target, uint8 chaseRadius)
    {

    }

  }
}
