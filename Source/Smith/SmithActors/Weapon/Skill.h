// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithCommandFormat.h"
#include "Skill.generated.h"

/**
 * 
 */
namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
	}
}

USTRUCT()
struct  SMITH_API FSkill
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString NAME;
	UPROPERTY(EditAnywhere)
	int32 VALUE;
	TSoftObjectPtr<UDataTable> SkillTable;
};
