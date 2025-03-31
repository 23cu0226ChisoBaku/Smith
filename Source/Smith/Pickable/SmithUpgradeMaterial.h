// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmithPickable.h"
#include "Params.h"
#include "ParamAbsorbable.h"
#include "ISmithItemWidgetParameterizable.h"
#include "ISmithBattleParameterizable.h"

#include "SmithUpgradeMaterial.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew,CollapseCategories)
class SMITH_API USmithUpgradeMaterial : public USmithPickable
																			, public IParamAbsorbable
																			, public ISmithItemWidgetParameterizable
																			, public ISmithBattleParameterizable
{
	GENERATED_BODY()

public:
	USmithUpgradeMaterial(const FObjectInitializer&);
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

public:
	FParams GetParam() override;
	void AddParam(FParams) override;
	
public:
	UTexture2D* GetIconImage() const override;
	FColor GetLabelColor() const override;
	FString GetName() const override;
	FString GetDescription() const override;

public:
	FParams GetParam_Interface() const override;

private:
	UPROPERTY(EditAnywhere, Category = ItemWidgetParameter)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, Category = ItemWidgetParameter, meta = (MultiLine = true))
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Parameter, meta = (AllowPrivateAccess = "true"))
	FParams Param;
	
private:
	FColor m_labelColor;
};
