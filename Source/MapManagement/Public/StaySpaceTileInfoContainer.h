// Fill out your copyright notice in the Description page of Project Settings.
/*

StaySpaceTileInfoContainer.h

Author : MAI ZHICONG

Description : 

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_STAYSPACE_TILEINFO
#define SMITH_MAP_STAYSPACE_TILEINFO

#include "CoreMinimal.h"
#include "TileInfoContainer.h"

//---------------------------------------
/*
									前方宣言
*/
//---------------------------------------
class ICanSetOnMap;
class ISmithMapEvent;

namespace UE::Smith
{
	namespace Map
	{
		//---------------------------------------
		/*
											前方宣言
		*/
		//---------------------------------------
		class IPickable;

		///
		/// @brief ものやキャラクターを置けるタイル情報コンテナ
		/// namespace UE::Smith::Map
		///
		class MAPMANAGEMENT_API FStaySpaceTileInfoContainer : public FTileInfoContainer
		{
			//---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
			public:
				/// @param ETileType タイルタイプ
				explicit FStaySpaceTileInfoContainer(ETileType);
				~FStaySpaceTileInfoContainer();

				//---------------------------------------
				/*
												コピー
				*/
				//---------------------------------------
				FStaySpaceTileInfoContainer(const FStaySpaceTileInfoContainer&);
				FStaySpaceTileInfoContainer& operator=(const FStaySpaceTileInfoContainer&);

				//---------------------------------------
				/*
												ムーブ
				*/
				//---------------------------------------
				FStaySpaceTileInfoContainer(FStaySpaceTileInfoContainer&&) noexcept;
				FStaySpaceTileInfoContainer& operator=(FStaySpaceTileInfoContainer&&) noexcept;

			//---------------------------------------
			/*
							パブリック関数(インターフェース)
			*/
			//---------------------------------------
			// FStaySpaceTileInfoContainer Interface
			#pragma region FStaySpaceTileInfoContainer Interface
			public:
				///
				/// @brief マップオブジェクトを設定する
				/// @param ICanSetOnMap マップに配置できるオブジェクト
				///
				void SetMapObj(ICanSetOnMap*);
				///
				/// @brief 拾えるオブジェクトを設定する
				/// @param ICanSetOnMap 拾えるオブジェクト
				///
				void SetPickable(IPickable*);
				void SetEvent(ISmithMapEvent*);
				///
				/// @brief マップオブジェクトを返す
				/// @return 何かがいたら	-> ここにいるマップオブジェクトのポインタ
				///					何もいなかったら	-> nullptr
				///
				ICanSetOnMap* GetMapObject() const;
				///
				/// @brief 拾えるオブジェクトを返す
				/// @return 何かがあったら	-> ここにある拾えるオブジェクトのポインタ
				///					何もなかったら	-> nullptr
				///
				IPickable* GetPickable() const;
				ISmithMapEvent* GetEvent() const;
				///
				/// @brief 拾えるオブジェクトを置けるか
				///	@return 置けたら	-> true 置けなかったら	-> false
				///
				bool CanPlacePickable() const;
			#pragma endregion FStaySpaceTileInfoContainer Interface
			// end of FStaySpaceTileInfoContainer Interface
			private:
				virtual bool isAbleToStayOn_Impl() const override;
				virtual void reset_Impl() override;
			private:
				/// @brief FStaySpaceTileInfoContainerImpl実装(pImplイディオム)
				class StaySpaceTileInfoImpl;
				TUniquePtr<StaySpaceTileInfoImpl> m_pImpl;
		};
	}
}

#endif