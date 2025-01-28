// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IPickable.h"
#include "SmithPickable.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class SMITH_API USmithPickable : public UObject, public IPickable
{
	GENERATED_BODY()
public:
	USmithPickable(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	bool OnPick(ICanPick*) override final;
	FString GetPickType() const override final;

private:
	virtual bool onPickImpl(ICanPick*) PURE_VIRTUAL(USmithPickable::onPickImpl, return false;);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PickableInfomation, meta = (AllowPrivateAccess = "true"))
	FString PickableTypeName;
	
};
