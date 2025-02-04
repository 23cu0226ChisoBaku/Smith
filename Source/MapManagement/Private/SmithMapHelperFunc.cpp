// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.cpp

Author : MAI ZHICONG

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/15 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#include "SmithMapHelperFunc.h"
#include "SmithSection.h"
#include "SmithMap.h"

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
    bool FSmithMapHelperFunc::IsInSameRoom(FSmithMap* map, uint8 x1, uint8 y1, uint8 x2, uint8 y2)
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

    bool FSmithMapHelperFunc::IsInSameSection(FSmithMap* map, uint8 x1, uint8 y1, uint8 x2, uint8 y2)
    {
      if (map == nullptr)
      {
        return false;
      }

      FSmithSection* section1 = map->GetSectionByCoord(x1, y1);
      FSmithSection* section2 = map->GetSectionByCoord(x2, y2);

      if (section1 == nullptr || section2 == nullptr)
      {
        return false;
      }

      return section1 == section2;

    }

    bool FSmithMapHelperFunc::DirectMapElementRotation(FSmithMap* map, FRotator& outRotation, uint8 x, uint8 y)
    {
      if (map == nullptr)
      {
        return false;
      }

      FSmithSection* section = map->GetSectionByCoord(x, y);
      if (section == nullptr || !section->HasRoom())
      {
        return false;
      }

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

      const uint8 distanceToLeft = localSectionCoordX - roomLeft;
      const uint8 distanceToRight = roomRight - localSectionCoordX;
      const uint8 distanceToTop = localSectionCoordY - roomTop;
      const uint8 distanceToBottom = roomBottom - localSectionCoordY;

      using namespace MapHelperFunc::Private;
      uint8 quadrant = 0u;

      if (distanceToRight < distanceToLeft)
      {
        outRotation = FRotator{0.0, 180.0, 0.0};
        if (distanceToBottom > distanceToTop) 
        {
          quadrant = SECOND_QUADRANT;
        }
        else if(distanceToBottom < distanceToTop)
        {
          quadrant = FIRST_QUADRANT;
        }
      }
      else if (distanceToRight > distanceToLeft)
      {
        outRotation = FRotator::ZeroRotator;
        if (distanceToBottom > distanceToTop) 
        {
          quadrant = THIRD_QUADRANT;
        }
        else if(distanceToBottom < distanceToTop)
        {
          quadrant = FOURTH_QUADRANT;
        }
      }
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
        return true;
      }

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
            outRotation = FRotator{0.0, 90.0, 0.0};
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
            outRotation = FRotator{0.0, 270.0, 0.0};
          }
          else
          {
            outRotation = FRotator{0.0, 180.0, 0.0};
          }
        }
        break;
      }
      return true;
    }
  }
}
