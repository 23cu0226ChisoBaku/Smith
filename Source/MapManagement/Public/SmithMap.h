// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMap.h

Author : MAI ZHICONG

Description : マップクラス

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP
#define SMITH_MAP

#include "CoreMinimal.h"

namespace UE::Smith
{
	namespace Map
	{
		//---------------------------------------
		/*
											前方宣言
		*/
		//---------------------------------------
		class FSmithRect;
		class FSmithSection;

		/// 
		/// @brief マップクラス 
		///
		class MAPMANAGEMENT_API FSmithMap
		{
			//---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
			public:
				FSmithMap();
				~FSmithMap();

			//---------------------------------------
			/*
							パブリック関数(インターフェース)
			*/
			//---------------------------------------
			// FSmithMap Interface
			#pragma region FSmithMap Interface
			public:
				///
				/// @brief                  マップを生成する（セクションは均等で生成される）
				/// @param row              マップにあるセクションの行数
				/// @param column           マップにあるセクションの列数
				/// @param widthPerSection  セクションの横幅
				/// @param heightPerSection セクションの縦幅
				/// @param sectionGap       セクションの間のギャップ
				/// @param defaultValue     マップのマスの初期データ
				///
				void GenerateMap(uint8 row, uint8 column, uint8 widthPerSection,uint8 heightPerSection, uint8 sectionGap, uint8 defaultValue);
				///
				/// @brief                  部屋を生成する
				/// @param sectionIdx       生成するセクションのインデックス
				/// @param roomMinWidth     部屋の最小横幅
				/// @param roomMaxWidth     部屋の最大横幅
				/// @param roomMinHeight    部屋の最小縦幅
				/// @param roomMaxHeight    部屋の最大縦幅
				/// @param defaultValue     部屋の初期データ
				///
				void GenerateRoom(uint8 sectionIdx, uint8 roomMinWidth, uint8 roomMaxWidth, uint8 roomMinHeight, uint8 roomMaxHeight, uint8 defaultValue);
				///
				/// @brief 部屋を接続する
				///
				void ConnectRooms(uint8 corridorData);
        ///
        /// @brief                  セクションの数を取得
        /// @return                 セクションの数
        ///
				int32 GetSectionCount() const;
        ///
        /// @brief                  セクションの行数を取得
        /// @return                 セクションの行数
        ///
				uint8 GetRow() const;
        ///
        /// @brief                  セクションの列数を取得
        /// @return                 セクションの列数
        ///
				uint8 GetColumn() const;
        ///
        /// @brief                  マップの矩形を取得
        /// @return                 マップの矩形
        ///
				FSmithRect GetMap() const;
				FSmithSection* GetSection(uint8 rowIdx, uint8 columnIdx) const;
				uint8 GetSectionLeft(uint8 columnIdx) const;
				uint8 GetSectionTop(uint8 rowIdx) const;
			#pragma endregion FSmithMap Interface
			// end of FSmithMap Interface
			private:
				/// @brief 実装クラス
				class MapImpl;
				TUniquePtr<MapImpl> m_pImpl;
		};
	}
}

#endif