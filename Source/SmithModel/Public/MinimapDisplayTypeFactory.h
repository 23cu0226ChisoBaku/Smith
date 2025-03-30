// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "MinimapDisplayType.h"
#include "IMinimapDisplayable.h"
#include "MinimapDisplayTypeFactory.generated.h"

UCLASS(Blueprintable, Abstract)
class SMITHMODEL_API UMinimapDisplayTypeFactory : public UObject
{
	GENERATED_BODY()

public:
	IMinimapDisplayable* GetDisplayable(EMinimapDisplayType);

private:
	UTexture2D* findProductFromDataTable(EMinimapDisplayType);

private:
	UPROPERTY()
	TMap<EMinimapDisplayType, TScriptInterface<IMinimapDisplayable>> m_typeCaches;

	UPROPERTY(EditAnywhere, meta = (RequiredAssetDataTags = "RowStructure=/Script/SmithModel.SmithMinimapDisplayProductRow"))
	TObjectPtr<UDataTable> FactoryProducts;
	
};
