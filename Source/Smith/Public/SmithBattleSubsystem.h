// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SmithBattleSubsystem.generated.h"

UCLASS()
class SMITH_API USmithBattleSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool ShouldCreateSubsystem(UObject* Outer) const override final;

/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override final;

	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override final;
	
	void YAYA()
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("YAYA"));
		}
	}
};
