// Fill out your copyright notice in the Description page of Project Settings.
/*

IEventPublishMediator.h

Author : MAI ZHICONG(バクチソウ)

Description : [ファイル説明]

Update History: [20xx/xx/xx] [コメント]

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEventPublishMediator.generated.h"

class USmithPickable;
class ICanSetOnMap;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEventPublishMediator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IEventPublishMediator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PublishPickUpEvent(ICanSetOnMap*, USmithPickable*) = 0;
};
