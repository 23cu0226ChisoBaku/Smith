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
struct  SMITH_API Skill
{
	GENERATED_BODY()
public:
	FString NAME;
	int32 VALUE;
}

};
