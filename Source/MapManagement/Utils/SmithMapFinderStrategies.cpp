// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapFinderStrategies.h"

#include "SmithMap.h"
#include "SmithSection.h"
#include "MapCoord.h"

namespace UE::Smith
{
  namespace Map
  {
    void MapFinderStrategy_Corner::operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const
    {
      if (Map == nullptr)
      {
        return;
      }
      
      OutCoords.Reset();
      const uint8 sectionRow = Map->GetRow();
      const uint8 sectionColumn = Map->GetColumn();
      for (uint8 row = 0u; row < sectionRow; ++row)
      {
        for (uint8 column = 0u; column < sectionColumn; ++column)
        {
          FSmithSection* section = Map->GetSection(row, column);
          if (section == nullptr || !section->HasRoom())
          {
            continue;
          }
  
          const uint8 sectionLeft = Map->GetSectionLeft(column);
          const uint8 sectionTop = Map->GetSectionTop(row);
          const uint8 roomLeft = section->GetRoomLeft() + sectionLeft;
          const uint8 roomRight = roomLeft + section->GetRoomWidth() - 1u;
          const uint8 roomTop = section->GetRoomTop() + sectionTop;
          const uint8 roomBottom = roomTop + section->GetRoomHeight() - 1u;
  
          OutCoords.Emplace(FMapCoord(roomLeft, roomTop));
          OutCoords.Emplace(FMapCoord(roomLeft, roomBottom));
          OutCoords.Emplace(FMapCoord(roomRight, roomTop));
          OutCoords.Emplace(FMapCoord(roomRight, roomBottom));
        }
      }		
    }

    void MapFinderStrategy_Random::operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const
    {
      if (Map == nullptr)
      {
        return;
      }
      
      OutCoords.Reset();
      const uint8 sectionRow = Map->GetRow();
      const uint8 sectionColumn = Map->GetColumn();
      for (uint8 row = 0u; row < sectionRow; ++row)
      {
        for (uint8 column = 0u; column < sectionColumn; ++column)
        {
          FSmithSection* section = Map->GetSection(row, column);
          if (section == nullptr || !section->HasRoom())
          {
            continue;
          }
  
          const uint8 sectionLeft = Map->GetSectionLeft(column);
          const uint8 sectionTop = Map->GetSectionTop(row);
          const uint8 roomLeft = section->GetRoomLeft() + sectionLeft;
          const uint8 roomRight = roomLeft + section->GetRoomWidth() - 1u;
          const uint8 roomTop = section->GetRoomTop() + sectionTop;
          const uint8 roomBottom = roomTop + section->GetRoomHeight() - 1u;
  
          for (uint8 roomCoordX = roomLeft; roomCoordX <= roomRight; ++roomCoordX)
          {
            for (uint8 roomCoordY = roomTop; roomCoordY <= roomBottom; ++roomCoordY)
            {
              OutCoords.Emplace(FMapCoord(roomCoordX, roomCoordY));
            }
          }
        }
      }
    }

    void MapFinderStrategy_CornerPerRoom::operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const
    {
      if (Map == nullptr)
      {
        return;
      }
      
      OutCoords.Reset();
      const uint8 sectionRow = Map->GetRow();
      const uint8 sectionColumn = Map->GetColumn();
      for (uint8 row = 0u; row < sectionRow; ++row)
      {
        for (uint8 column = 0u; column < sectionColumn; ++column)
        {
          FSmithSection* section = Map->GetSection(row, column);
          if (section == nullptr || !section->HasRoom())
          {
            continue;
          }
  
          const uint8 sectionLeft = Map->GetSectionLeft(column);
          const uint8 sectionTop = Map->GetSectionTop(row);
          const uint8 roomLeft = section->GetRoomLeft() + sectionLeft;
          const uint8 roomRight = roomLeft + section->GetRoomWidth() - 1u;
          const uint8 roomTop = section->GetRoomTop() + sectionTop;
          const uint8 roomBottom = roomTop + section->GetRoomHeight() - 1u;
  
          int32 rand = FMath::RandRange(0,3);
          switch (rand)
          {
            case 0:
            {
              OutCoords.Emplace(FMapCoord(roomLeft, roomTop));  
            }
            break;
            case 1:
            {
              OutCoords.Emplace(FMapCoord(roomLeft, roomBottom));
            }
            break;
            case 2:
            {
              OutCoords.Emplace(FMapCoord(roomRight, roomTop));
            }
            break;
            case 3:
            {
              OutCoords.Emplace(FMapCoord(roomRight, roomBottom));
            }
            break;
          }
        }
      }
    }
  }
}
