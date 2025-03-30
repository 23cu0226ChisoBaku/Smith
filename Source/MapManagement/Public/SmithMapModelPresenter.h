// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAP_MODEL_PRESENTER
#define SMITH_MAP_MODEL_PRESENTER

struct FMapCoord;
class ISmithMapModelRequester;

namespace UE::Smith
{
	namespace Map
	{
		struct FSmithMapDataModel;
		
		class FSmithMapModelPresenter
		{
			using ThisClass = typename FSmithMapModelPresenter;

			public:

				FSmithMapModelPresenter(TSharedPtr<FSmithMapDataModel> MapData, ISmithMapModelRequester* MapModelRequester);
				~FSmithMapModelPresenter();

				FSmithMapModelPresenter(ThisClass&& Other) noexcept = default;
				ThisClass& operator=(ThisClass&& Other) noexcept = default;
			
			public:

				void UpdateMapObjectCoord(AActor* MapObject, FMapCoord OriginMapCoord);
				bool IsObjectOnMap(AActor* MapObject) const;
				void PlaceMapObject(AActor* MapObject, FMapCoord OriginMapCoord);

			private:

				FSmithMapModelPresenter(const ThisClass& Other) = delete;
				ThisClass& operator=(const ThisClass& Other) = delete;

			private:

				class PresenterImpl;
				TUniquePtr<PresenterImpl> m_pimpl;

		};
	}
}

#endif