// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithConsumeItem.h"
#include "SmithHPItem.generated.h"

/**
 * 
 */
UCLASS()
class SMITH_API USmithHPItem : public USmithConsumeItem
{
	GENERATED_BODY()

public:
	USmithHPItem(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	double GetRecoveryPercentage() const;
	// TODO
	void SetRecoveryPercentage(double percentage);

private:
	void useImpl(IItemUseable*) override;

private:
	UPROPERTY(EditAnywhere)
	double RecoveryPercentage;
};
