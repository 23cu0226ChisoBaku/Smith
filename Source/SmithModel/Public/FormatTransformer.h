// Fill out your copyright notice in the Description page of Project Settings.
/*

FormatTransformer.h

Author : MAI ZHICONG

Description : Format Transformer class

Update History: 2024/11/29 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"

#include "SmithCommandFormat.h"
#include "MapCoord.h"

#ifndef SMITH_FORMAT_TRANS
#define SMITH_FORMAT_TRANS

namespace UE::Smith
{
	inline namespace Battle
	{
		class SMITHMODEL_API FFormatTransformer
		{
			public:
				static UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord> FormatToMapCoord(FSmithCommandFormat format, FMapCoord cnMapCoord);
		};
	}
}

#endif