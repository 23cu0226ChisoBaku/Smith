// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ICanSetOnMap;

namespace UE::Smith
{
	namespace Map
	{
		struct FSmithMapDataModel;

		class MAPMANAGEMENT_API FSmithMapObjDeployDirector
		{
			public:
				FSmithMapObjDeployDirector();
				~FSmithMapObjDeployDirector();
				FSmithMapObjDeployDirector(FSmithMapObjDeployDirector&&) noexcept;
				FSmithMapObjDeployDirector& operator=(FSmithMapObjDeployDirector&&) noexcept;

			private:
				FSmithMapObjDeployDirector(const FSmithMapObjDeployDirector&) = delete;
				FSmithMapObjDeployDirector& operator=(const FSmithMapObjDeployDirector&) = delete;

			public:
				void AssignMap(TSharedPtr<FSmithMapDataModel>);
				void DeployMapObj(ICanSetOnMap*,uint8 x, uint8 y);
			
			private:
				class DeployDirectorImpl;
				TUniquePtr<DeployDirectorImpl> m_pImpl;
		};
	}
}
