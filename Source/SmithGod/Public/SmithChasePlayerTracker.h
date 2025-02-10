// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ITargetTracker.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithChasePlayerTracker.generated.h"

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapManager;
	}
}
enum class EDirection : uint8;
class ICanSetOnMap;
/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithChasePlayerTracker : public UObject, public ITargetTracker
{
	GENERATED_BODY()

public:
	USmithChasePlayerTracker(const FObjectInitializer&);

public:
	void SetupTracker(TSharedPtr<UE::Smith::Map::FSmithMapManager>);

public:
	bool TrackTarget(EDirection& outDirection, ICanSetOnMap* chaser, uint8 chaseRadius) override final;

private:
	TWeakPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
};
