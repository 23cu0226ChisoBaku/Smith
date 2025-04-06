// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "DropRatioMode.h"
#include "ILootGeneratable.h"
#include "SmithPickableLootHandle.h"
#include "SmithLootGameInstanceSubsystem.generated.h"

UCLASS()
class SMITHGAMEPLAY_API USmithLootGameInstanceSubsystem : public UGameInstanceSubsystem, public ILootGeneratable
{
	GENERATED_BODY()

	struct LootListElement
	{
		TArray<FSmithPickableLootHandle> LootHandleList;
		EDropRatioMode Mode;
	};
	
public:
	USmithLootGameInstanceSubsystem();
	virtual void BeginDestroy() override;
	void AssignLootList(UDataTable*);

public:

	IPickable* GetLoot(const FString& EnemyTypeName) const override final;

private:

	TMap<FString, LootListElement> m_lootList;
};
