// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Command Format

Author : MAI ZHICONG

Description : Command Format(Use by ActionCommand)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_CMD_FORMAT
#define SMITH_CMD_FORMAT

#include "CoreMinimal.h"
#include "Dimension2Array.h"
#include "MapCoord.h"

namespace UE::Smith
{
	inline namespace Battle
	{
		class SMITH_API FSmithCommandFormat
		{
			public:
				FSmithCommandFormat(const int8* srcData, const size_t dataCnt, const uint64 row, const uint64 column);
				~FSmithCommandFormat();

				FSmithCommandFormat(const FSmithCommandFormat&);
				FSmithCommandFormat& operator=(const FSmithCommandFormat&);

				FSmithCommandFormat(FSmithCommandFormat&&) noexcept;
				FSmithCommandFormat& operator=(FSmithCommandFormat&&) noexcept;

			public:
				UE::MLibrary::MDataStructure::TDimension2Array<int8> GetFormatArray() const;
				uint64 GetRow() const;
				uint64 GetColumn() const;

			private:
				void setCNCoord();
			private:
				UE::MLibrary::TDimension2Array<int8> m_formatArr;
				FMapCoord m_cnCoord;
		};
	}
}

#endif