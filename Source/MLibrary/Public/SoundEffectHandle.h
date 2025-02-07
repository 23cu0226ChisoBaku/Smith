// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef MLIBRARY_SE_HANDLE
#define MLIBRARY_SE_HANDLE

#include "CoreMinimal.h"

struct MLIBRARY_API FSoundEffectHandle
{
public:
	constexpr FSoundEffectHandle()
		: UniqueIdentifier{}
	{}
	FSoundEffectHandle(const FString& identifierStr)
		: UniqueIdentifier(identifierStr)
	{

	}
	~FSoundEffectHandle()
	{
		UniqueIdentifier.Invalidate();
	}
public:
	FGuid UniqueIdentifier;
};

#endif
