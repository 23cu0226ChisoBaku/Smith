// Fill out your copyright notice in the Description page of Project Settings.
/*

Dimension2Array.h

Author : MAI ZHICONG

Description : 2-Dimension array template class

Update History: 2024/11/27 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef M_DIMENSION_TWO_ARRAY
#define M_DIMENSION_TWO_ARRAY

#include "CoreMinimal.h"

#include "Dimension1Array.h"

/**
 * 
 */

namespace UE::MLibrary
{
	inline namespace MDataStructure
	{
		///
		/// @brief 二次元配列
		/// @param T データの型
		///
		template<typename T>
		class SMITH_API TDimension2Array
		{

		//---------------------------------------
		/*
							エイリアスやアサーション
		*/
		//---------------------------------------
		using ElementType = T;
		using ArrayType = TDimension1Array<ElementType>;
		//---------------------------------------
		/*
										ctorとdtor
		*/
		//---------------------------------------
			public:
				TDimension2Array()
					: m_elemArr(nullptr)
					, m_length(0)
				{ }
				TDimension2Array(uint64 row, uint64 column)
					: m_elemArr(new ArrayType*[row])
					, m_length(row)
				{
					for(uint64 i = 0; i < m_length; ++i)
					{
						m_elemArr[i] = new ArrayType(column);
					}
				}

				TDimension2Array(const TDimension2Array& other)
				{
					this->m_elemArr = new ArrayType*[other.m_length];
					this->m_length = other.m_length;

					uint64_t columnSize = other.At_ReadOnly(0).Length();
					for(uint64 i = 0; i < m_length; ++i)
					{
						this->m_elemArr[i] = new ArrayType(columnSize);
					}
				}
				TDimension2Array& operator=(const TDimension2Array& other)
				{
					this->m_elemArr = new ArrayType*[other.m_length];
					this->m_length = other.m_length;

					uint64_t columnSize = other.At_ReadOnly(0).Length();
					for(uint64 i = 0; i < m_length; ++i)
					{
						this->m_elemArr[i] = new ArrayType(columnSize);
					}

					return *this;
				}

				TDimension2Array(TDimension2Array&& other) noexcept
				{
					*this = MoveTemp(other);
					other.m_elemArr = nullptr;
					other.m_length = 0;
				}
				TDimension2Array& operator=(TDimension2Array&& other) noexcept
				{
					*this = MoveTemp(other);
					other.m_elemArr = nullptr;
					other.m_length = 0;

					return *this;
				}

				~TDimension2Array()
				{
					dispose();
				}

		//---------------------------------------
		/*
						パブリック関数(クラス専用)
		*/
		//---------------------------------------
			public:
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]のコンスト参照
				const ArrayType& At_ReadOnly(uint64 row) const &
				{
					return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]のコンスト参照
				const ArrayType& At(uint64 row) const &
				{
					return at_impl(row);
				}

				const ElementType& At_ReadOnly(uint64 row, uint64 column) const &
				{
					return at_impl(row,column);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				ArrayType& At(uint64 row) &
				{
					return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				ArrayType& At(uint64 row) &&
				{
					return at_impl(row);
				}
				/// @brief 配列の行数を返す(読み込み専用)
				/// @return 配列の行数
				uint64 Row() const
				{
					return m_length;
				}
				/// @brief 配列の列数を返す(読み込み専用)
				/// @return 配列の列数				
				uint64 Column() const
				{
					return m_elemArr[0]->Length();
				}
				/// @brief 配列の長さを返す(読み込み専用)
				/// @return 配列の長さ
				uint64 Length() const
				{
					return m_length * m_elemArr[0]->Length();
				}
		//---------------------------------------
		/*
						パブリック関数(インターフェース)
		*/
		//---------------------------------------
			public:

		//---------------------------------------
		/*
						オペレーターオーバーロード
		*/
		//---------------------------------------
			public:
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]のコンスト参照
				const ArrayType& operator[](uint64 row) const&
				{
						return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				ArrayType& operator[](uint64 row) &
				{
						return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				ArrayType& operator[](uint64 row) &&
				{
						return at_impl(row);
				}
		// Private
		#pragma region Private
			private:
				void dispose() noexcept
				{
					if (m_elemArr != nullptr)
					{
						for(uint64 i = 0; i < m_length; ++i)
						{
							delete m_elemArr[i];
						}
						delete[] m_elemArr;
					}
					memset(this, 0, sizeof(this));
				}
				ArrayType& at_impl(uint64 row)
				{
					check(row < m_length)

					return *m_elemArr[row];
				}

				ElementType& at_impl(uint64 row, uint64 column)
				{
					check(row < m_length && column < m_elemArr[0]->Length())

					return m_elemArr[row][column];

				}

			private:
				ArrayType** m_elemArr;
				uint64 m_length;

		#pragma endregion
		// Private
		};
	}
}

#endif