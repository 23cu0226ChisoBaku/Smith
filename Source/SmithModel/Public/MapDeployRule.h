// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
enum class EMapDeployRule : uint8
{
	Random = 0u UMETA(DisplayName="Random Deployment"),
	Corner UMETA(DisplayName="Corner Deployment"),
	Corner_Per_Room UMETA(DisplayName="Corner Per Room Deployment"),
	Sides_With_Corner UMETA(DisplayName="Side Deployment"),
	Sides_Without_Corner UMETA(DisplayName="Side without corner Deployment"),
};

