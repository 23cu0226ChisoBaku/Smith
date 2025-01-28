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
	void OnPick(ICanPick*) override final;
	FString GetPickType() const override final;

private:
	virtual void onPickImpl(ICanPick*) PURE_VIRTUAL(USmithPickable::onPickImpl);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PickableInfomation, meta = (AllowPrivateAccess = "true"))
	FString PickableTypeName;
	
};
