// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/15 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#include "SmithMapHelperLibrary.h"

#include "SmithSection.h"
#include "SmithMap.h"
#include "SmithRect.h"
#include "MapCoord.h"
#include "MapDeployRule.h"
#include "SmithMapFinderStrategies.h"

#include "MLibrary.h"

/// @brief cpp専用名前空間
namespace MapHelperFunc::Private
{
  constexpr uint8 FIRST_QUADRANT = 1u;
  constexpr uint8 SECOND_QUADRANT = 2u;
  constexpr uint8 THIRD_QUADRANT = 3u;
  constexpr uint8 FOURTH_QUADRANT = 4u;
}

namespace UE::Smith
{
  namespace Map
  {
    struct MapFinderOperator
    {
      using ThisClass = typename MapFinderOperator;

      private:
        struct FinderConcept
        {
          virtual ~FinderConcept() = default;

          virtual void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const = 0;
        };

        template<typename FinderStrategy>
        struct FinderModel : public FinderConcept
        {
          FinderModel(FinderStrategy Strategy)
            : StrategyEntity(::MoveTemp(Strategy))
          {}

          void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const override final
          {
            StrategyEntity(Map, OutCoords);
          }

          FinderStrategy StrategyEntity;
        };

      public:

        template<typename StrategyType>
        explicit MapFinderOperator(StrategyType Strategy)
          : m_pimpl(::MakeShared<FinderModel<StrategyType>>(::MoveTemp(Strategy)))
        {}

        ~MapFinderOperator()
        {
          m_pimpl.Reset();
        }

        MapFinderOperator(const ThisClass& Other) = default;
        ThisClass& operator=(const ThisClass& Other) = default;
        MapFinderOperator(ThisClass&& Other) noexcept = default;
        ThisClass& operator=(ThisClass&& Other) noexcept = default;

        void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const
        {
          (*m_pimpl)(Map, OutCoords);
        }

      private:

        TSharedPtr<FinderConcept> m_pimpl;
    };

    bool FSmithMapHelperLibrary::IsInSameRoom(FSmithMap* map, uint8 x1, uint8 y1, uint8 x2, uint8 y2)
    {
      if (map == nullptr)
      {
        return false;
      }

      if (!IsInSameSection(map, x1, y1, x2, y2))
      {
        return false;
      }

      FSmithSection* section1 = map->GetSectionByCoord(x1, y1);
      FSmithSection* section2 = map->GetSectionByCoord(x2, y2);

      if (section1 == nullptr || section2 == nullptr)
      {
        return false;
      }

      return true;
    }

    bool FSmithMapHelperLibrary::IsInSameSection(FSmithMap* map, uint8 x1, uint8 y1, uint8 x2, uint8 y2)
    {
      if (map == nullptr)
      {
        return false;
      }

      FSmithSection* section1 = map->GetSectionByCoord(x1, y1);
      FSmithSection* section2 = map->GetSectionByCoord(x2, y2);

      if ((section1 == nullptr) || (section2 == nullptr))
      {
        return false;
      }

      return section1 == section2;
    }

    void FSmithMapHelperLibrary::DirectMapElementRotation(FSmithMap* map, FRotator& outRotation, uint8 x, uint8 y)
    {
      outRotation = FRotator::ZeroRotator;
      if (map == nullptr)
      {
        return;
      }

      // マップ要素のある/いるセクション
      FSmithSection* section = map->GetSectionByCoord(x, y);
      if (section == nullptr || !section->HasRoom())
      {
        return;
      }

      // 必要なデータを計算
      const uint8 mapColumn = map->GetColumn();
      const uint8 sectionIdx = section->GetSectionIdx();
      const uint8 rowIdx = sectionIdx / mapColumn;
      const uint8 columnIdx = sectionIdx % mapColumn;
      const uint8 sectionLeft = map->GetSectionLeft(columnIdx);
      const uint8 sectionTop = map->GetSectionTop(rowIdx);
      const uint8 localSectionCoordX = x - sectionLeft;
      const uint8 localSectionCoordY = y - sectionTop;
      const uint8 roomLeft = section->GetRoomLeft();
      const uint8 roomTop = section->GetRoomTop();
      const uint8 roomRight = roomLeft + section->GetRoomWidth() - 1u;
      const uint8 roomBottom = roomTop + section->GetRoomHeight() - 1u;

      // 要素と四つの辺の距離を計算
      const uint8 distanceToLeft = localSectionCoordX - roomLeft;
      const uint8 distanceToRight = roomRight - localSectionCoordX;
      const uint8 distanceToTop = localSectionCoordY - roomTop;
      const uint8 distanceToBottom = roomBottom - localSectionCoordY;

      using namespace MapHelperFunc::Private;
      // 要素のある/いる象限を探す
      // 部屋を基準にしているため、UEのXY軸の方向と違う
      uint8 quadrant = 0u;

      // 第一・第四象限又は間に
      if (distanceToRight < distanceToLeft)
      {
        outRotation = FRotator{0.0, 180.0, 0.0};
        if (distanceToBottom > distanceToTop) 
        {
          quadrant = FIRST_QUADRANT;
        }
        else if(distanceToBottom < distanceToTop)
        {
          quadrant = FOURTH_QUADRANT;
        }
      }
      // 第二・第三象限又は間に
      else if (distanceToRight > distanceToLeft)
      {
        outRotation = FRotator::ZeroRotator;
        if (distanceToBottom > distanceToTop) 
        {
          quadrant = SECOND_QUADRANT;
        }
        else if(distanceToBottom < distanceToTop)
        {
          quadrant = THIRD_QUADRANT;
        }
      }
      // 軸の上に
      else
      {
        if (distanceToBottom > distanceToTop) 
        {
          outRotation = FRotator{0.0, 90.0, 0.0};
        }
        else
        {
          outRotation = FRotator{0.0, 270.0, 0.0};
        }
      }

      if (quadrant == 0)
      {
        return;
      }

      // 壁に近い方向を探し、壁に背を向けて配置する
      switch (quadrant)
      {
        case FIRST_QUADRANT:
        {
          if (distanceToTop > distanceToRight)
          {
            outRotation = FRotator{0.0, 180.0, 0.0};
          }
          else
          {
            outRotation = FRotator{0.0, 90, 0.0};
          }
        }
        break;
        case SECOND_QUADRANT:
        {
          if (distanceToTop > distanceToLeft)
          {
            outRotation = FRotator::ZeroRotator;
          }
          else
          {
            outRotation = FRotator{0.0, 90.0, 0.0};
          }
        }
        break;
        case THIRD_QUADRANT:
        {
          if (distanceToBottom > distanceToLeft)
          {
            outRotation = FRotator::ZeroRotator;
          }
          else
          {
            outRotation = FRotator{0.0, 270.0, 0.0};
          }
        }
        break;
        case FOURTH_QUADRANT:
        {
          if (distanceToBottom > distanceToRight)
          {
            outRotation = FRotator{0.0, 180.0, 0.0};
          }
          else
          {
            outRotation = FRotator{0.0, 270.0, 0.0};
          }
        }
        break;
      }
    }

    int32 FSmithMapHelperLibrary::GetMapCoordsByRule(FSmithMap* map, EMapDeployRule rule, TArray<FMapCoord>& outCoords)
    {
      if (map == nullptr)
      {
        return -1;
      }
      
      using enum EMapDeployRule;
      static const TMap<EMapDeployRule, const MapFinderOperator> mapFinderStrategyContext = 
      {
        {Corner, MapFinderOperator{MapFinderStrategy_Corner{}}},        
        {Random, MapFinderOperator{MapFinderStrategy_Random{}}},        
        {Corner_Per_Room, MapFinderOperator{MapFinderStrategy_CornerPerRoom{}}}  
      };

      if (!mapFinderStrategyContext.Contains(rule))
      {
        return -1;
      }

      mapFinderStrategyContext[rule](map, outCoords);

      return outCoords.Num();
    }

    uint8 FSmithMapHelperLibrary::GetRoomCount(FSmithMap* map)
    {
      if (map == nullptr)
      {
        return 0u;
      }
      
      uint8 count = 0u;

      for (int32 sectionRow = 0; sectionRow < map->GetRow(); ++sectionRow)
      {
        for (int32 sectionColumn = 0; sectionColumn < map->GetColumn(); ++sectionColumn)
        {
          FSmithSection* section = map->GetSection(sectionRow, sectionColumn);
          if (section == nullptr || !section->HasRoom())
          {
            continue;
          }

          ++count;
        }
      }

      return count;
    }

    bool FSmithMapHelperLibrary::IsSameTileNearby(FSmithMap* map, const FMapCoord& originCoord, FMapCoord& outDiffTileCoord)
    {
      if (map == nullptr)
      {
        return false;
      }

      if (map->GetMapWidth() < originCoord.x + 1
        ||map->GetMapHeight() < originCoord.y + 1)
      {
        return false;
      }

      FSmithRect mapRect = map->GetMap();

      uint8 tileData = mapRect.GetRect(originCoord.x, originCoord.y);

      if (tileData != mapRect.GetRect(originCoord.x + 1, originCoord.y))
      {
        outDiffTileCoord.x = originCoord.x + 1;
        outDiffTileCoord.y = originCoord.y;
        return false;
      }

      if (tileData != mapRect.GetRect(originCoord.x - 1, originCoord.y))
      {
        outDiffTileCoord.x = originCoord.x - 1;
        outDiffTileCoord.y = originCoord.y;
        return false;
      }

      if (tileData != mapRect.GetRect(originCoord.x, originCoord.y + 1))
      {
        outDiffTileCoord.x = originCoord.x;
        outDiffTileCoord.y = originCoord.y + 1;
        return false;
      }

      if (tileData != mapRect.GetRect(originCoord.x, originCoord.y - 1))
      {
        outDiffTileCoord.x = originCoord.x;
        outDiffTileCoord.y = originCoord.y - 1;
        return false;
      }

      return true;
    }

  }

}
