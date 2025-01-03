// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapBuilder.h"
#include "SmithMapBluePrint.h"
#include "SmithMap.h"

namespace UE::Smith
{
  namespace Map
  {
    FSmithMapBuilder::FSmithMapBuilder()
    { }

    FSmithMapBuilder::~FSmithMapBuilder()
    { }

    bool FSmithMapBuilder::Build(FSmithMap* map, FSmithMapBluePrint blueprint)
    {
      if (map == nullptr)
      {
        return false;
      }

      map->GenerateMap(blueprint.SectionRow, blueprint.SectionColumn, blueprint.SectionWidth, blueprint.SectionHeight, blueprint.SectionGap, StaticCast<uint8>(blueprint.DefaultSectionTileType));
      map->GenerateRooms(blueprint.RoomMinWidth, blueprint.RoomMaxWidth, blueprint.RoomMinHeight, blueprint.RoomMaxHeight, StaticCast<uint8>(blueprint.DefaultRoomTileType));
      map->ConnectRooms(StaticCast<uint8>(blueprint.DefaultCorridorTileType));
      return true;
    }
  }
}
