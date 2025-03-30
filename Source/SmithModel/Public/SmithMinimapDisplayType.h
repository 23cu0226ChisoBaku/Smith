// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "IMinimapDisplayable.h"

#include "SmithMinimapDisplayType.generated.h"

/**
 * 
 */
UCLASS()
class SMITHMODEL_API USmithMinimapDisplayType : public UObject , public IMinimapDisplayable
{
	GENERATED_BODY()

public:

	USmithMinimapDisplayType();
	void SetTexture(UTexture2D*);

	//---Begin of UObject Interface
	virtual void BeginDestroy() override;
	//---End of UObject Interface;

public:

	UTexture2D* GetMinimapDisplayTexture_Implementation() override;

private:
	UPROPERTY()
	TObjectPtr<UTexture2D> m_displayTexture;
};
