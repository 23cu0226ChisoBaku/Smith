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
		private:
			class TDimension2ArrayRowArr
			{
				friend class TDimension2Array<T>;
				private:
					TDimension2ArrayRowArr(ElementType* src, uint64 length)
						: m_rowArr(src)
						, m_length(length)
					{	}
				public:
					~TDimension2ArrayRowArr()
					{
						memset(this, 0, sizeof(this));
					}
				public:
					const ElementType& At_Readonly(uint64 idx) const&
					{
						return at_impl(idx);
					}
					const ElementType& At(uint64 idx) const&
					{
						return at_impl(idx);
					}
					ElementType& At(uint64 idx) &
					{
						return at_impl(idx);
					}
					ElementType& At(uint64 idx) &&
					{
						return at_impl(idx);
					}
					uint64 Length() const
					{
						return m_length;
					}

				public:
					const ElementType& operator[](uint64 idx) const&
					{
						return at_impl(idx);
					}
					ElementType& operator[](uint64 idx) &
					{
						return at_impl(idx);
					}
					ElementType& operator[](uint64 idx) &&
					{
						return at_impl(idx);
					}
				private:
					ElementType& at_impl(uint64 idx) 
					{
						check(m_rowArr != nullptr)
						check(m_length != 0)

						return m_rowArr[idx];
					}
				private:
					ElementType* m_rowArr;
					uint64 m_length;
			};

		using RowArray = TDimension2ArrayRowArr;
		//---------------------------------------
		/*
										ctorとdtor
		*/
		//---------------------------------------
			public:
				TDimension2Array()
					: m_elemArr(nullptr)
					, m_row(0)
					, m_column(0)
				{ }
				TDimension2Array(const uint64 row, const uint64 column)
					: m_elemArr(nullptr)
					, m_row(row)
					, m_column(column)
				{
					check( ((m_row * m_column) > 0) )

					m_elemArr = new ElementType[m_row * m_column];
				}

				TDimension2Array(const ElementType* src, const size_t srcSize, const uint64 row, const uint64 column)
					: TDimension2Array(row, column)
				{
					check(src != nullptr)
					// サイズが０じゃない　かつ　サイズが （データのサイズ * 行の数 * 列の数）
					check(((srcSize != 0) && (srcSize == (sizeof(ElementType) * row * column))))

					memcpy_s(m_elemArr, srcSize, src, srcSize);

				}

				TDimension2Array(const TDimension2Array& other)
				{
					this->m_elemArr = new ElementType[other.Length()];
					this->m_row = other.m_row;
					this->m_column = other.m_column;
					const size_t memSize = sizeof(ElementType) * other.m_row * other.m_column;
					memcpy_s(m_elemArr, memSize, other.m_elemArr, memSize);
				}
				TDimension2Array& operator=(const TDimension2Array& other)
				{
					if (this != &other)
					{
						this->m_elemArr = new ElementType[other.Length()];
						this->m_row = other.m_row;
						this->m_column = other.m_column;
						const size_t memSize = sizeof(ElementType) * other.m_row * other.m_column;
						memcpy_s(m_elemArr, memSize, other.m_elemArr, memSize);
					}

					return *this;
				}

				TDimension2Array(TDimension2Array&& other) noexcept
				{
					this->m_elemArr = ::MoveTemp(other.m_elemArr);
					this->m_row = ::MoveTemp(other.m_row);
					this->m_column = ::MoveTemp(other.m_column);

					other.m_elemArr = nullptr;
					other.m_row = 0;
					other.m_column = 0;
				}
				TDimension2Array& operator=(TDimension2Array&& other) noexcept
				{
					if (this != &other)
					{
						this->m_elemArr = ::MoveTemp(other.m_elemArr);
						this->m_row = ::MoveTemp(other.m_row);
						this->m_column = ::MoveTemp(other.m_column);

						other.m_elemArr = nullptr;
						other.m_row = 0;
						other.m_column = 0;
					}

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
				const RowArray At_ReadOnly(uint64 row) const &
				{
					return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]のコンスト参照
				const RowArray At(uint64 row) const &
				{
					return at_impl(row);
				}
				/// @brief 配列の行を返す(書き込み可能)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				RowArray At(uint64 row) &
				{
					return at_impl(row);
				}
				/// @brief 配列の行を返す(書き込み可能)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				RowArray At(uint64 row) &&
				{
					return at_impl(row);
				}

				/// @brief 二次元配列の値を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @param column 列のインデックス
				/// @return 配列[行のインデックス][列のインデックス]のコンスト参照
				const ElementType& At_ReadOnly(uint64 row, uint64 column) const &
				{
					return at_impl(row, column);
				}
				/// @brief 二次元配列の値を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @param column 列のインデックス
				/// @return 配列[行のインデックス][列のインデックス]のコンスト参照
				const ElementType& At(uint64 row, uint64 column) const &
				{
					return at_impl(row, column);
				}
				/// @brief 二次元配列の値を返す(書き込み可能)
				/// @param row 行のインデックス
				/// @param column 列のインデックス
				/// @return 配列[行のインデックス][列のインデックス]の参照
				ElementType& At(uint64 row, uint64 column) &
				{
					return at_impl(row, column);
				}
				/// @brief 二次元配列の値を返す(書き込み可能)
				/// @param row 行のインデックス
				/// @param column 列のインデックス
				/// @return 配列[行のインデックス][列のインデックス]の参照
				ElementType& At(uint64 row, uint64 column) &&
				{
					return at_impl(row, column);
				}
				/// @brief 配列の行数を返す(読み込み専用)
				/// @return 配列の行数
				uint64 Row() const
				{
					return m_row;
				}
				/// @brief 配列の列数を返す(読み込み専用)
				/// @return 配列の列数				
				uint64 Column() const
				{
					return m_column;
				}
				/// @brief 配列の長さを返す(読み込み専用)
				/// @return 配列の長さ
				uint64 Length() const
				{
					return m_row * m_column;
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
				const RowArray operator[](uint64 row) const&
				{
						return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				RowArray operator[](uint64 row) &
				{
						return at_impl(row);
				}
				/// @brief 配列の行を返す(読み込み専用)
				/// @param row 行のインデックス
				/// @return 配列[行のインデックス]の参照
				RowArray operator[](uint64 row) &&
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
						delete[] m_elemArr;
					}

					memset(this, 0, sizeof(this));
				}
				RowArray at_impl(uint64 row)
				{
					check(m_elemArr != nullptr)
					check(row < m_row)

					return RowArray(m_elemArr + row * m_column, m_column);
				}

				ElementType& at_impl(uint64 row, uint64 column)
				{
					check(m_elemArr != nullptr)
					check(row < m_row && column < m_column)

					return m_elemArr[row * m_column + column];

				}
		//---------------------------------------
		/*
						 スタティック変数(private)
		*/
		//---------------------------------------
			private:

		//---------------------------------------
		/*
						 スタティック変数(public)
		*/
		//---------------------------------------
			public:
				static const TDimension2Array<ElementType> EmptyArray;
			
			private:
				ElementType* m_elemArr;
				uint64 m_row;
				uint64 m_column;

		#pragma endregion
		// Private
		};

		template<typename T>
		const TDimension2Array<T> TDimension2Array<T>::EmptyArray = TDimension2Array<T>();
	}
}

#endif