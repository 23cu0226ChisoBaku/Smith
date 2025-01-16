// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMap;

		class MAPMANAGEMENT_API FSmithMapHelperFunc
		{
		// Cannot Create Instance	
		private:
			FSmithMapHelperFunc() = delete;
			~FSmithMapHelperFunc() = delete;

		public:
			static bool IsInSameRoom(FSmithMap*, uint8 x1, uint8 y1, uint8 x2, uint8 y2);
			static bool IsInSameSection(FSmithMap*, uint8 x1, uint8 y1, uint8 x2, uint8 y2);
		};
	}
}
