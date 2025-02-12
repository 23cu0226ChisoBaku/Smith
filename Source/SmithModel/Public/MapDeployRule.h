// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EMapDeployRule : uint8
{
	Random UMETA(DisplayName="Random Deployment"),
	Corner UMETA(DisplayName="Corner Deployment"),
	Sides_With_Corner UMETA(DisplayName="Side Deployment"),
	Sides_Without_Corner UMETA(DisplayName="Side without corner Deployment"),
};

