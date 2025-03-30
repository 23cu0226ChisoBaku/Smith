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

#ifndef SMITH_FORMAT_TRANS
#define SMITH_FORMAT_TRANS

struct FMapCoord;
enum class EDirection : uint8;

namespace UE::MLibrary::MDataStructure
{
	template<typename T>
	class TDimension2Array;
}

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;

		class SMITHMODEL_API FFormatTransformer
		{
			public:
			
				static UE::MLibrary::MDataStructure::TDimension2Array<FMapCoord> FormatToMapCoord(const FSmithCommandFormat& format, FMapCoord cnMapCoord);
				static FSmithCommandFormat GetRotatedFormat(const FSmithCommandFormat& src, EDirection direction);

			private:
				static FSmithCommandFormat getRotatedFormatImpl(const FSmithCommandFormat& src, EDirection direction);
		};
	}
}

#endif