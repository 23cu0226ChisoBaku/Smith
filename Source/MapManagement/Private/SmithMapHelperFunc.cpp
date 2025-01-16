// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapHelperFunc.h"
#include "SmithSection.h"
#include "SmithMap.h"
#include "SmithMapHelperFunc.h"

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

      if (!IsInSameSection(map, x1, y1, x2, y2));
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
  }
}
