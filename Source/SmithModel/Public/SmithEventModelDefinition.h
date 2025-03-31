// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithEventModelDefinition.generated.h"


USTRUCT()
struct FSmithEventModelData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, Category = "Smith|EventData", meta = (ShortTooltip = "成功した時のメッセージ"))
  FString SucceededMessage;

  UPROPERTY(EditDefaultsOnly, Category = "Smith|EventData", meta = (ShortTooltip = "失敗した時のメッセージ"))
  FString FailedMessage;
  
};







UCLASS(BlueprintType, Blueprintable, Const)
class SMITHMODEL_API USmithEventModelDefinition : public UDataAsset
{
  GENERATED_BODY()

public:
  
  UPROPERTY(EditDefaultsOnly, Category = "Smith|EventData", meta = (MustImplement = "SmithEventLogger"))
  TSubclassOf<UObject> EventClass;

  UPROPERTY(EditDefaultsOnly, Category = "Smith|EventData")
  FSmithEventModelData EventData;
};
