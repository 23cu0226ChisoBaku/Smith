// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "FormatType.h"

#include "FormatInfo_Import.generated.h"

USTRUCT(DisplayName = "FormatArrayInfo")
struct SMITHMODEL_API FFormatInfo_Import : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESmithFormatType Type;
};
