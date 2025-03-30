// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithSection.h

Author : MAI ZHICONG(バクチソウ)

Description : セクションクラス（マップの基本単位）

Update History: 2024/12/30 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_SECTION
#define SMITH_SECTION

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
    
    ///
    /// @brief セクションクラス
    /// namespace UE::Smith::Map
    ///　
    class FSmithSection
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        ///
        /// @brief             セクションコンストラクタ
        /// @param sectionIdx  セクションのインデックス
        ///
        FSmithSection(uint8 sectionIdx);
        ~FSmithSection();

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithSection(const FSmithSection&) = delete;
        FSmithSection& operator=(const FSmithSection&) = delete;
      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithSection Interface
      #pragma region FSmithSection Interface
      public:
        /// 
        /// @brief              セクションを作成
        /// @param left         セクションの左端
        /// @param top          セクションの上端
        /// @param right        セクションの右端
        /// @param bottom       セクションの下端
        /// @param defaultData  セクションに入れる初期データ
        /// 
        void GenerateSection(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 defaultData);
        /// 
        /// @brief              部屋を作成
        /// @param left         部屋の左端
        /// @param top          部屋の上端
        /// @param right        部屋の右端
        /// @param bottom       部屋の下端
        /// @param defaultData  部屋に入れる初期データ
        /// 
        void GenerateRoom(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 defaultData);
        ///
        /// @brief              部屋があるかを確認
        /// @return             部屋がある-> true 部屋がない-> false
        ///
        bool HasRoom() const;		
        ///
        /// @brief              セクションの横幅を取得
        /// @return             横幅
        ///
        uint8 GetWidth() const;
        ///
        /// @brief              セクションの縦幅を取得
        /// @return             縦幅
        ///
        uint8 GetHeight() const;
        ///
        /// @brief              セクションにある部屋の横幅を取得
        /// @return             部屋があったら部屋の横幅、なかったら０
        ///
        uint8 GetRoomWidth() const;
        ///
        /// @brief              セクションにある部屋の縦幅を取得
        /// @return             部屋があったら部屋の縦幅、なかったら０
        ///
        uint8 GetRoomHeight() const;
        ///
        /// @brief              セクションにある部屋の横座標を返す（セクションのローカル座標）
        /// @return             部屋があったらX座標（ローカル）、なかったら０
        ///
        uint8 GetRoomLeft() const;
        ///
        /// @brief              セクションにある部屋の縦座標を返す（セクションのローカル座標）
        /// @return             部屋があったらY座標（ローカル）、なかったら０
        ///
        uint8 GetRoomTop() const;
        ///
        /// @brief              セクションのインデックスを取得
        /// @return             インデックス
        ///
        uint8 GetSectionIdx() const;
        ///
        /// @brief              セクションにある部屋の矩形を取得
        /// @return             部屋があったら部屋の矩形、なかったら空っぽの矩形
        ///
        FSmithRect GetRoomRect() const;
        ///
        /// @brief              セクションの矩形を取得
        /// @return             セクションの矩形
        ///
        FSmithRect GetSectionRect() const;
      #pragma endregion FSmithSection Interface
      // end of FSmithSection Interface
      private:
        ///
        /// @brief 実装クラス(pImplイディオム)
        ///
        class SectionImpl;
        TUniquePtr<SectionImpl> m_pImpl;
    };
  }
}

#endif
