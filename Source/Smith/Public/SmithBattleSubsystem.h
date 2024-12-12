// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ITurnManageableWrapper.h"
#include "TurnPriority.h"
#include "SmithBattleSubsystem.generated.h"

enum class ETurnPriority : uint8;


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
	
	void StartBattle();
	
	void YAYA()
	{
		FString str{};
		if (GEngine != nullptr)
		{
			for (const auto& pair : m_priorityLists)
			{
				for (const auto& actor : pair.Value.Elements)
				{
					str.Reset();
					FString enumName {}; 
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETurnPriority"), true);
					if (EnumPtr != nullptr)
					{
						enumName.Append(EnumPtr->GetNameByValue(StaticCast<int64>(pair.Key)).ToString());
					}

					str.Append(enumName);
					str.Append(Cast<AActor>(actor)->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, str);
				}
			}
		}
	}

private:
	UPROPERTY()
	TMap<ETurnPriority, FITurnManageableWrapper> m_priorityLists;
};
