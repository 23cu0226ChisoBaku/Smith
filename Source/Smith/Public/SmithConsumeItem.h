// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithPickable.h"
#include "SmithConsumeItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class SMITH_API USmithConsumeItem : public USmithPickable
{
	GENERATED_BODY()

public:
	USmithConsumeItem(const FObjectInitializer&);
	virtual void BeginDestroy() override;
	
public:
	void Use();

private:
	virtual void useImpl() PURE_VIRTUAL(USmithConsumeItem::useImpl);
	
};
