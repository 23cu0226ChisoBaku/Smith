// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IParamInitializer.h"
#include "SmithTowerEnemyParamInitializer.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithTowerEnemyParamInitializer : public UObject, public IParamInitializer
{
	GENERATED_BODY()

public:
	USmithTowerEnemyParamInitializer();
	virtual void BeginDestroy() override;
	void AssignEnemyParamList(UDataTable* paramList);

public:
	FParams Initialize(SmallGolem, int32 currentLevel);
	FParams Initialize(Dragon, int32 currentLevel);
	FParams Initialize(HerbGolem,int32 currentLevel);
	
private:
	TMap<FString, FParams> m_paramList;
};
