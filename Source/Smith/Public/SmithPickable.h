// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IPickable.h"
#include "ISmithBattleLogger.h"
#include "SmithPickable.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, Blueprintable)
class SMITH_API USmithPickable : public UObject, public IPickable
															 , public ISmithBattleLogger
{
	GENERATED_BODY()
public:
	USmithPickable(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	FString GetPickType() const override final;
	virtual FString GetName_Log() const override final;
	virtual EBattleLogType GetType_Log() const PURE_VIRTUAL(USmithPickable::GetType_Log, return EBattleLogType::None;);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PickableInfomation, meta = (AllowPrivateAccess = "true"))
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PickableInfomation, meta = (AllowPrivateAccess = "true"))
	FString PickableTypeName;
	
};
