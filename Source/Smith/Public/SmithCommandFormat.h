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

/**
 * 
 */
class SMITH_API FSmithCommandFormat
{
	public:
		FSmithCommandFormat();
		~FSmithCommandFormat();
		FSmithCommandFormat(uint32 row, uint32 column);

		FSmithCommandFormat(const FSmithCommandFormat&);
		FSmithCommandFormat& operator=(const FSmithCommandFormat&);

		FSmithCommandFormat(FSmithCommandFormat&&);
		FSmithCommandFormat& operator=(FSmithCommandFormat&&);

	public:
		uint32 GetRow() const;
		uint32 GetColumn() const;

	public:
		
	private:
		UE::MLibrary::TDimension2Array<int8> m_formatArr;
		uint32 m_row;
		uint32 m_column;
};

#endif