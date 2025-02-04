// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapEventDirector.h"
#include "SmithMap.h"
#include "SmithSection.h"
#include "SmithMapDataModel.h"

#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMapEventDirector::EventDirectorImpl
    {
      using Model = typename FSmithMapDataModel;
      public:
        EventDirectorImpl()
          : m_model(nullptr)
        {}
        ~EventDirectorImpl()
        {
          m_model.Reset();
        }
      public:
        void AssignMap(TSharedPtr<Model> model)
        {
          m_model = model;
        }
        void DirectNextLevelEventCoord(uint8& outX, uint8& outY)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          { 
            MDebug::LogError("failed to InitNextLevelEvent --- model invalid");
            return;
          }

          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
          if (!map_shared.IsValid())
          {
            MDebug::LogError("failed to InitNextLevelEvent --- map invalid");
            return;
          }

          TArray<uint8> sectionIdx{};
          sectionIdx.Reserve(map_shared->GetSectionCount());

          if (map_shared->GetSectionCount() == 0)
          {
            MDebug::LogError("failed to InitNextLevelEvent --- map section num invalid");
            return;
          }

          outX = 0u;
          outY = 0u;

          for (uint8 i = 0u; i < map_shared->GetSectionCount(); ++i)
          {
            sectionIdx.Emplace(i);
          }

          FUECollectionsLibrary::Shuffle(sectionIdx);
          for(int32 i = 0; i < sectionIdx.Num(); ++i)
          {
            const uint8 sectionRow = sectionIdx[i] / map_shared->GetColumn();
            const uint8 sectionColumn = sectionIdx[i] % map_shared->GetColumn();
            FSmithSection* sectionPtr = map_shared->GetSection(sectionRow, sectionColumn);
            if (sectionPtr == nullptr || !sectionPtr->HasRoom())
            {
              continue;
            }

            // 2*2以下の部屋に階段を置かないようにする
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

            FUECollectionsLibrary::Shuffle(eventRandomPlaceCoords);
            for (int32 j = 0; j < eventRandomPlaceCoords.Num(); ++j)
            {
              const FMapCoord mapCoord = eventRandomPlaceCoords[j];

              if (!model_shared->StaySpaceTable.Contains(mapCoord))
              {
                continue;
              }

              if (canPlaceEvent(mapCoord))
                {
                  outX = mapCoord.x;
                  outY = mapCoord.y;
                  return;
                }
            }
          }
        }
        // TODO need check out of bounds;
        bool canPlaceEvent(FMapCoord coord, bool bPlaceOnCrossroad = false)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (model_shared->StaySpaceTable[coord]->GetMapObject() != nullptr 
              || model_shared->StaySpaceTable[coord]->GetEvent() != nullptr
             )
          {
            return false;
          }
    
          if (!bPlaceOnCrossroad)
          {
            // 4だったら交差点
            int32 crossCount = 0;
            {
              const FMapCoord coord_north{StaticCast<uint8>(coord.x + 1u), coord.y};
              if (model_shared->StaySpaceTable.Contains(coord_north))
              {
                ++crossCount;
              }
            }
            {
              const FMapCoord coord_east{StaticCast<uint8>(coord.x), StaticCast<uint8>(coord.y + 1u)};
              if (model_shared->StaySpaceTable.Contains(coord_east))
              {
                ++crossCount;
              }
            }
            {
              const FMapCoord coord_south{StaticCast<uint8>(coord.x - 1u), StaticCast<uint8>(coord.y)};
              if (model_shared->StaySpaceTable.Contains(coord_south))
              {
                ++crossCount;
              }
            }
            {
              const FMapCoord coord_west{StaticCast<uint8>(coord.x), StaticCast<uint8>(coord.y - 1u)};
              if (model_shared->StaySpaceTable.Contains(coord_west))
              {
                ++crossCount;
              }
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

          return true;
        }
      private:
        TWeakPtr<Model> m_model;
    };
    FSmithMapEventDirector::FSmithMapEventDirector()
      : m_pImpl(::MakeUnique<EventDirectorImpl>())
    { }

    FSmithMapEventDirector::~FSmithMapEventDirector()
    {
      m_pImpl.Reset();
    }

    FSmithMapEventDirector::FSmithMapEventDirector(FSmithMapEventDirector&& rhs)
      : m_pImpl(::MoveTemp(rhs.m_pImpl))
    { }

    FSmithMapEventDirector& FSmithMapEventDirector::operator=(FSmithMapEventDirector&& rhs)
    {
      if (this != &rhs)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(rhs.m_pImpl);
      }

      return *this;
    }

    void FSmithMapEventDirector::AssignMap(TSharedPtr<FSmithMapDataModel> model)
    {
      m_pImpl->AssignMap(model);
    }
    void FSmithMapEventDirector::DirectNextLevelEventCoord(uint8& outX, uint8& outY)
    {
      m_pImpl->DirectNextLevelEventCoord(outX, outY);
    }
  }
}
