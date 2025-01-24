// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ISendableLog.h"

class SMITH_API TestSendLog : public ISendableLog
{
public:	
	// Sets default values for this actor's properties
	TestSendLog();

	public:
	FString GetName() override final;
	FColor GetColor() override final;
};
