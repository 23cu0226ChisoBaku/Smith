// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_UPGRADE_ITEM_HANDLE
#define SMITH_UPGRADE_ITEM_HANDLE

#include "CoreMinimal.h"
#include "../Weapon/Params.h"

struct FSmithUpgradeItemHandle
{
	UTexture2D* IconImage2D;
	FString Name;
	FString Description;
	FParams Param;

  FSmithUpgradeItemHandle();
  FSmithUpgradeItemHandle(UTexture2D* icon, const FString& name, const FString& description, const FParams& param);
};

#endif
