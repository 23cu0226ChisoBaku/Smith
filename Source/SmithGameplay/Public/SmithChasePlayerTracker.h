// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "ITargetTracker.h"

#include "UObject/WeakInterfacePtr.h"

#include "SmithChasePlayerTracker.generated.h"

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapSystem;
	}
}
enum class EDirection : uint8;
/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithChasePlayerTracker : public UObject, public ITargetTracker
{
	GENERATED_BODY()

public:
	USmithChasePlayerTracker(const FObjectInitializer&);

public:
	void SetupTracker(const TSharedPtr<UE::Smith::Map::FSmithMapSystem>& MapManager);

public:
	bool TrackTarget(/* OUT */EDirection& outDirection, AActor* Chaser, uint8 chaseRadius) override final;

private:

	TWeakPtr<UE::Smith::Map::FSmithMapSystem> m_mapSys;
};
