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

#include "Dimension2Array.h"
#include "MapCoord.h"

namespace UE::Smith
{
	inline namespace Battle
	{
		enum ESmithFormatType : int8
		{
			NO_EFFECT = -1,
			CENTER_NO_EFFECT = 0,
			EFFECT = 1,
			CENTER_EFFECT = 2,
		};

		class FSmithCommandFormat
		{
			public:
				FSmithCommandFormat();
				FSmithCommandFormat(const ESmithFormatType* srcData, const size_t dataCnt, const uint64 row, const uint64 column);
				~FSmithCommandFormat();

				FSmithCommandFormat(const FSmithCommandFormat&);
				FSmithCommandFormat& operator=(const FSmithCommandFormat&);

				FSmithCommandFormat(FSmithCommandFormat&&) noexcept;
				FSmithCommandFormat& operator=(FSmithCommandFormat&&) noexcept;

			public:
				void SetupFormat(const ESmithFormatType* srcData, const size_t srcSize, const uint64 row, const uint64 column);
				UE::MLibrary::MDataStructure::TDimension2Array<ESmithFormatType> GetFormatArray() const;
				FMapCoord GetCNCoord() const;
				uint64 GetRow() const;
				uint64 GetColumn() const;

			private:
				void setCNCoord();
			private:
				UE::MLibrary::MDataStructure::TDimension2Array<ESmithFormatType> m_formatArr;
				FMapCoord m_cnCoord;
		};
	}
}

#endif