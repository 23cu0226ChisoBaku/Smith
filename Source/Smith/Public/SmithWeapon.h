// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEnhanceable.h"
#include "Params.h"
#include "ISmithBattleLogger.h"
#include "SmithWeapon.generated.h"

class UTexture2D;

struct FSmithWeaponInfoHandle
{
	FString Name;
	FParams Param;
	int32 Level;
	UTexture2D* Image2D;

	FSmithWeaponInfoHandle()
		: Name{}
		, Param{}
		, Level(1)
		, Image2D(nullptr)
	{ }
	FSmithWeaponInfoHandle(FString name, FParams param, int32 level, UTexture2D* image2D)
		: Name(name)
		, Param(param)
		, Level(level)
		, Image2D(image2D)
	{ }
};
/**
 * 
 */
UCLASS(EditInlineNew)
class SMITH_API USmithWeapon : public UObject, public IEnhanceable, public ISmithBattleLogger
{
	GENERATED_BODY()

public:

	USmithWeapon(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	//---Begin of IEnhanceable Interface
	virtual void Upgrade(IParamAbsorbable*) override;
	virtual void OnUpgraded() override;
	//---End of IEnhanceable Interface
	
	void SetParam(FParams);
	FParams GetParam() const;
	int32 GetLevel() const;
	FSmithWeaponInfoHandle GetHandle() const
	{
		FSmithWeaponInfoHandle handle(Name, WeaponParam, m_weaponLevel, WeaponImage2D);
		return handle;
	}

	FString GetName_Log() const override;
	EBattleLogType GetType_Log() const override;

// TODO
public:
	TMulticastDelegate<void(FParams)> OnParamUpdated;
	
private:
	UPROPERTY(EditAnywhere, Category = "Smith|Weapon")
	FParams WeaponParam;
	UPROPERTY(EditAnywhere, Category = "Smith|Weapon")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Smith|Weapon")
	TObjectPtr<UTexture2D> WeaponImage2D;

	int32 m_weaponLevel;
	
};
