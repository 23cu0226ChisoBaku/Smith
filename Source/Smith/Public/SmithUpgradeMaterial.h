// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithPickable.h"
#include "Params.h"
#include "ParamAbsorbable.h"
#include "SmithUpgradeMaterial.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew,CollapseCategories)
class SMITH_API USmithUpgradeMaterial : public USmithPickable, public IParamAbsorbable
{
	GENERATED_BODY()

public:
	USmithUpgradeMaterial(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	FParams GetParam() override;
	void AddParam(FParams) override;

private:
	void onPickImpl(ICanPick*) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Parameter, meta = (AllowPrivateAccess = "true"))
	FParams Param;
};
