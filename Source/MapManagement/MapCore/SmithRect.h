// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithRect.h

Author : MAI ZHICONG(バクチソウ)

Description : MapManagement モジュール専用矩形

Update History: 2024/12/29 作成
                2025/01/30 Description修正

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#ifndef SMITH_RECT
#define SMITH_RECT

namespace UE::MLibrary
{
  namespace MDataStructure
  {
    template<typename ElementType> struct TDimension2ArrayHandle;
  }
}

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief 矩形クラス（最大256*256）　矩形に入れるデータ型：uint8(unsigned char)
    /// namespace UE::Smith::Map
    ///　
    class FSmithRect
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithRect();
        ~FSmithRect();

      //---------------------------------------
      /*
                        コピー
      */
      //---------------------------------------
      public:
        FSmithRect(const FSmithRect&);
        FSmithRect& operator=(const FSmithRect&);

      //---------------------------------------
      /*
                        ムーブ
      */
      //---------------------------------------
      public:
        FSmithRect(FSmithRect&&) noexcept;
        FSmithRect& operator=(FSmithRect&&) noexcept;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithRect Interface
      #pragma region FSmithRect Interface
        public:
          ///
          /// @brief            	矩形を作成する
          /// @param width      	矩形の横幅
          /// @param height     	矩形の縦幅
          /// @param defaultData	矩形に入れるデフォルトのデータ
          ///
          void GenerateRect(uint8 width, uint8 height, uint8 defaultData);
          ///
          /// @brief            	矩形にデータを入れ替える
          /// @param x          	矩形のXインデックス(横)
          /// @param y          	矩形のYインデックス(縦)
          /// @param data					入れるデータ
          ///
          void SetRect(uint8 x, uint8 y, uint8 data);
          ///
          /// @brief            	矩形の一定の範囲内のデータを入れ替える
          /// @param left       	矩形の左端(横)
          /// @param top        	矩形の上端(縦)
          /// @param right      	矩形の右端(横)
          /// @param bottom     	矩形の下端(縦)
          /// @param data					入れるデータ
          ///
          void FillRect(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 data);
          ///
          /// @brief            	矩形の横幅を取得
          /// @return							横幅
          ///
          uint8 GetWidth() const;
          ///
          /// @brief            	矩形の縦幅を取得
          /// @return							縦幅
          ///
          uint8 GetHeight() const;
          ///
          /// @brief							矩形のデータを取得
          /// @param x          	矩形のXインデックス(横)
          /// @param y          	矩形のYインデックス(縦)
          /// @return           	[x,y]のデータ
          ///          
          uint8 GetRect(uint8 x, uint8 y) const;
          ///
          /// @brief							矩形の範囲外（xが横幅を超えたもしくはyが縦幅を超えた）かを判断する
          /// @param x          	矩形のXインデックス(横)
          /// @param y          	矩形のYインデックス(縦)
          /// @return           	範囲外 -> true; 範囲内 -> false
          ///    				
          bool IsOutOfBounds(uint8 x, uint8 y) const;
      #pragma endregion FSmithRect Interface
      // end of FSmithRect Interface

      private:
        ///
        /// @brief 実装クラス(pImplイディオム)
        ///
        class RectImpl;
        TUniquePtr<RectImpl> m_pImpl;
    };
  }
}

#endif