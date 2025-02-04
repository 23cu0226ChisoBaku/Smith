// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#ifndef SMITH_MAP_EVENT_DIRECTOR
#define SMITH_MAP_EVENT_DIRECTOR

/**
 * 
 */
namespace UE::Smith
{
	namespace Map
	{
		struct FSmithMapDataModel;

		class MAPMANAGEMENT_API FSmithMapEventDirector
		{
			public:
				FSmithMapEventDirector();
				~FSmithMapEventDirector();

				FSmithMapEventDirector(FSmithMapEventDirector&&);
				FSmithMapEventDirector& operator=(FSmithMapEventDirector&&);

			private:
				FSmithMapEventDirector(const FSmithMapEventDirector&) = delete;
				FSmithMapEventDirector& operator=(const FSmithMapEventDirector&) = delete;

			public:
				void AssignMap(TSharedPtr<FSmithMapDataModel>);
				void DirectNextLevelEventCoord(uint8& outX, uint8& outY);

			private:
				class EventDirectorImpl;
				TUniquePtr<EventDirectorImpl> m_pImpl;
		};
	}
}

#endif