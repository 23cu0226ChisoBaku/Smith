// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MapObjType.h"
#include "SmithMapObjDisplayRow.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SMITHMODEL_API FSmithMapObjDisplayRow : public FTableRowBase
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  EMapObjType ObjectType;
  UPROPERTY(EditAnywhere)
  TObjectPtr<UTexture2D> ObjDisplayTexture;
};
