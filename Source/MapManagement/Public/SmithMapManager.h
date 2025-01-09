// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FMapCoord;
class IAttackable;
class ICanSetOnMap;

namespace UE::Smith
{
	namespace Battle
	{
    class FSmithCommandFormat;
    enum class EMoveDirection : uint8;
  }
}

struct FSmithMapBluePrint;
struct FSmithMapConstructionBluePrint;
struct FSmithEnemyGenerateBluePrint;

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMap;

    class MAPMANAGEMENT_API FSmithMapManager final
    {
      public:
        FSmithMapManager();
        ~FSmithMapManager();
      
      public:
        void InitMap(UWorld*, const FSmithMapBluePrint&, const FSmithMapConstructionBluePrint&);
        void InitMapObjs(UWorld*, AActor* player, const FSmithEnemyGenerateBluePrint&);
        void DeployMapObj(ICanSetOnMap*, uint8 x, uint8 y);
        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap*, const UE::Smith::Battle::FSmithCommandFormat&);
        void MoveMapObj(ICanSetOnMap*, UE::Smith::Battle::EMoveDirection, uint8 moveDistance, FVector&);

      private:
        class MapMgrImpl;
        TUniquePtr<MapMgrImpl> m_pImpl;
    };
  }
}