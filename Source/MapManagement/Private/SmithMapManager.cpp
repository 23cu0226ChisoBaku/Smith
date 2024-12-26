// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithMapManager.h"
#include "MapCoord.h"
#include "SmithMapObj.h"

#define ReturnIfTrue(expression)    \
        {                           \
          if (!!(expression))       \
          {                         \
            return;                 \
          }                         \
        }

#define ReturnIfFalse(expression)  ReturnIfTrue(expression)     

namespace Smith::Map::Private
{
  /// @brief アクターのユニークIDを計算する
  /// @param actor 
  /// @return ユニークID(uint32) (0 if actor is invalid)
  uint32 CalcUniqueID(AActor* actor)
  {
    if (!::IsValid(actor))
    {
      return 0;
    }

    const FString uniqueStr = actor->GetName() + FString::FromInt(actor->GetUniqueID());
    return GetTypeHash(FGuid{uniqueStr});
  }
}


namespace Smith::Map
{
  SmithMapManager::SmithMapManager()
    : m_curtMap(UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord>::EmptyArray)
    , m_onMapObjects({})
  { }

  SmithMapManager::~SmithMapManager()
  {
    m_onMapObjects.Empty();
  }

  void SmithMapManager::AssignMap(const FMapCoord* mapData, uint64 row, uint64 column)
  {
    using namespace UE::MLibrary::MDataStructure;
    check((mapData != nullptr));
    check(((row * column) != 0));

    m_onMapObjects.Empty();

    const size_t mapDataSize = sizeof(FMapCoord) * row * column;

    m_curtMap = TDimension2Array<FMapCoord>(mapData, mapDataSize, row, column);
  }

  void SmithMapManager::AddMapObj(FSmithMapObj* mapObj)
  {
    ReturnIfTrue(mapObj == nullptr || !(mapObj->IsValid()))

    const uint32 uniqueID = GetTypeHash(mapObj->GetUniqueID());
    if (m_onMapObjects.Contains(uniqueID))
    {
      return;
    }

    m_onMapObjects.Emplace(uniqueID, mapObj);
  }

  void SmithMapManager::UpdateMapObjCoord(AActor* const actor, FMapCoord newCoord)
  {
    using namespace Smith::Map::Private;

    ReturnIfTrue(!::IsValid(actor));

    const uint32 uniqueID = CalcUniqueID(actor);
    if (!m_onMapObjects.Contains(uniqueID))
    {
      return;
    }

    if ((m_onMapObjects[uniqueID] != nullptr) && m_onMapObjects[uniqueID]->IsValid())
    {
      m_onMapObjects[uniqueID]->SetCoord(newCoord);
    }
  } 
}
