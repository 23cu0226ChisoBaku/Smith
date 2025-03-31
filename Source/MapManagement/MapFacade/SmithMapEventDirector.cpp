// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapEventDirector.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップイベントを配置する前に向きなど情報を調整するクラス

Update History: 2025/02/04 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#include "SmithMapEventDirector.h"
#include "SmithMap.h"
#include "SmithSection.h"
#include "SmithMapDataModel.h"
#include "ItemGenerationListRow.h"
#include "SmithMapHelperLibrary.h"
#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @FSmithMapEventDirector 実装クラス
    /// 
    class FSmithMapEventDirector::EventDirectorImpl
    {
      //---------------------------------------
      /*
                        エイリアス
      */
      //---------------------------------------
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
        void AssignMapDataModel(TSharedPtr<Model> model)
        {
          m_model = model;
        }
        void DirectNextLevelEventCoord(uint8& outX, uint8& outY)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          { 
            MDebug::LogError("failed to DirectNextLevelEventCoord --- model invalid");
            return;
          }

          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
          if (!map_shared.IsValid())
          {
            MDebug::LogError("failed to DirectNextLevelEventCoord --- map invalid");
            return;
          }

          if (map_shared->GetSectionCount() == 0)
          {
            MDebug::LogError("failed to DirectNextLevelEventCoord --- map section num invalid");
            return;
          }

          TArray<uint8> sectionIdx{};
          sectionIdx.Reserve(map_shared->GetSectionCount());

          outX = 0u;
          outY = 0u;

          // 全てのセクションからランダムで一つを決める
          for (uint8 i = 0u; i < map_shared->GetSectionCount(); ++i)
          {
            sectionIdx.Emplace(i);
          }

          FUECollectionsLibrary::Shuffle(sectionIdx);
          for(int32 i = 0; i < sectionIdx.Num(); ++i)
          {
            // セクションを取得
            const uint8 sectionRow = sectionIdx[i] / map_shared->GetColumn();
            const uint8 sectionColumn = sectionIdx[i] % map_shared->GetColumn();
            FSmithSection* sectionPtr = map_shared->GetSection(sectionRow, sectionColumn);
            // セクションの有効性チェックかつ部屋があるかをチェック
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
            // 部屋の左右両辺の座標
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
        bool GetDeployableCoord(EMapDeployRule rule, uint8& outX, uint8& outY)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (!model_shared.IsValid())
          {
            return false;
          }

          TSharedPtr<FSmithMap> map_shared = model_shared->Map.Pin();
          if (!map_shared.IsValid())
          {
            return false;
          }

          TArray<FMapCoord> deployableCoords{};
          
          // TODO Need efficiency refactoring
          FSmithMapHelperLibrary::GetMapCoordsByRule(map_shared.Get(), rule, deployableCoords);
          FUECollectionsLibrary::Shuffle(deployableCoords);

          for (const auto& coord : deployableCoords)
          {
            if (model_shared->StaySpaceTable.Contains(coord) && model_shared->StaySpaceTable[coord]->GetEvent() == nullptr)
            {
              outX = coord.x;
              outY = coord.y;
              return true;
            }
          }

          return false;

        }
        private:
        ///
        /// @brief 座標にイベントを置けるかをチェック
        ///
        bool canPlaceEvent(FMapCoord coord, bool bPlaceOnCrossroad = false)
        {
          TSharedPtr<Model> model_shared = m_model.Pin();
          if (model_shared->StaySpaceTable[coord]->GetEvent() != nullptr)
          {
            return false;
          }

          // 交差点（上下左右四つのタイルも地面）に配置する時
          if (!bPlaceOnCrossroad)
          {
            // 4になると交差点
            int32 crossCount = 0;
            // 北方向
            {
              const FMapCoord coord_north(coord.x + 1u, coord.y);
              if (model_shared->StaySpaceTable.Contains(coord_north))
              {
                ++crossCount;
              }
            }
            // 東方向
            {
              const FMapCoord coord_east(coord.x, coord.y + 1u);
              if (model_shared->StaySpaceTable.Contains(coord_east))
              {
                ++crossCount;
              }
            }
            // 南方向
            {
              const FMapCoord coord_south(coord.x - 1u, coord.y);
              if (model_shared->StaySpaceTable.Contains(coord_south))
              {
                ++crossCount;
              }
            }
            // 西方向
            {
              const FMapCoord coord_west(coord.x, coord.y - 1u);
              if (model_shared->StaySpaceTable.Contains(coord_west))
              {
                ++crossCount;
              }
            }

            if (crossCount == 4)
            {
              return false;
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

    void FSmithMapEventDirector::AssignMapDataModel(TSharedPtr<FSmithMapDataModel> model)
    {
      m_pImpl->AssignMapDataModel(model);
    }
    void FSmithMapEventDirector::DirectNextLevelEventCoord(uint8& outX, uint8& outY)
    {
      m_pImpl->DirectNextLevelEventCoord(outX, outY);
    }
    bool FSmithMapEventDirector::GetDeployableCoord(EMapDeployRule rule, uint8& outX, uint8& outY)
    {
      return m_pImpl->GetDeployableCoord(rule, outX, outY);
    }
  }
}
