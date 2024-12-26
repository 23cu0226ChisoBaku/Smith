// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructure/Dimension2Array.h"

struct FMapCoord;

namespace Smith::Map
{
  class FSmithMapObj;
}

/**
* 
*/

namespace Smith::Map
{
  class MAPMANAGEMENT_API SmithMapManager final
  {
  public:
    SmithMapManager();
    ~SmithMapManager();
  

  public:
    void AssignMap(const FMapCoord* mapData, uint64 row, uint64 column);
    void AddMapObj(FSmithMapObj*);
    void UpdateMapObjCoord(AActor* const, FMapCoord);

  private:
    UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord> m_curtMap;
    TMap<uint32, TSharedPtr<FSmithMapObj>> m_onMapObjects;
  };
}