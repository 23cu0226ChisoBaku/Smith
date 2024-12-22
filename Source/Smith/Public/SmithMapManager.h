// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dimension2Array.h"
#include "SmithCommandFormat.h"

struct FMapCoord;
class FSmithMapObj;

/**
* 
*/

namespace Smith::Map
{
  class SMITH_API SmithMapManager final
  {
  public:
    SmithMapManager();
    ~SmithMapManager();

  public:
    void AssignMap(const FMapCoord* mapData, uint64 row, uint64 column);
    void AddMapObj(FSmithMapObj*);
    void UpdateMapObj(FSmithMapObj*);

  private:
    UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord> m_curtMap;
    TMap<uint64, TWeakPtr<FSmithMapObj>> m_onMapObjects;
  };
}