// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnhanceable.h"
#include "Params.h"
#include "SmithWeapon.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class SMITH_API USmithWeapon : public UObject, public IEnhanceable
{
	GENERATED_BODY()
public:
	USmithWeapon(const FObjectInitializer&);
	virtual void BeginDestroy() override;
public:
	virtual void Upgrade(IParamAbsorbable*) override;
	void SetParam(FParams);
	FParams GetParam() const;
	int32 GetLevel() const;

private:
	UPROPERTY(EditAnywhere, BlueprintType, Category = WeaponParameter, meta = (AllowPrivateAccess = "true"))
	FParams WeaponParam;
	int32 m_weaponLevel;
	
};
