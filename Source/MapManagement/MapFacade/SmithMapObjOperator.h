// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjOperator.h

Author : MAI ZHICONG(バクチソウ)

Description : マップオブジェクトを操作する(移動など)クラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_OPERATOR
#define SMITH_MAP_OPERATOR

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IEventRegister;
class UObject;
struct FAttackableInfoHandle;
struct FMapCoord;
enum class EDirection : uint8;
namespace UE::Smith
{
  namespace Battle
  {
    class FSmithCommandFormat;
  }
}

namespace UE::Smith
{
  namespace Map
  {
    //---------------------------------------
    /*
                      前方宣言
    */
    //---------------------------------------
    class FSmithMap;
    struct FSmithMapDataModel;

    class FSmithMapObjOperator
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapObjOperator();
        ~FSmithMapObjOperator();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapObjOperator(FSmithMapObjOperator&&) noexcept;
        FSmithMapObjOperator& operator=(FSmithMapObjOperator&&) noexcept;

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithMapObjOperator(const FSmithMapObjOperator&) = delete;
        FSmithMapObjOperator& operator=(const FSmithMapObjOperator&) = delete;
      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithMapObserver Interface
      #pragma region FSmithMapObjOperator Interface
      public:
        void AssignEventRegister(IEventRegister*);
        ///
        ///	@brief													マップモデルを登録する
        /// @param	FSmithMapDataModel			マップモデル
        /// @param	originCoord_World				マップ原点座標
        /// @param	mapTileSize							マップタイルサイズ
        ///
        void AssignMapDataModel(TSharedPtr<FSmithMapDataModel>);
        ///
        /// @brief 													攻撃できるオブジェクトを探す
        /// @param outActors 								攻撃できるオブジェクト(IAttackable)が全て入っているコンテナ
        /// @param ICanSetOnMap							ターゲット
        /// @param FSmithCommandFormat			攻撃フォーマット
        ///
        void FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor*, const UE::Smith::Battle::FSmithCommandFormat&);
        // TODO
        void FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor*, const UE::Smith::Battle::FSmithCommandFormat&, uint8 offsetToLeft, uint8 offsetToTop);
        ///
        /// @brief 													オブジェクトを移動する
        /// @param ICanSetOnMap							移動するターゲットオブジェクト
        /// @param EMoveDirection						移動方向
        /// @param moveDistance 						移動距離
        /// @param FVector									移動先の座標
        ///
        void MoveMapObj(AActor*, EDirection, uint8 moveDistance, FVector&);
        void AddUpdateMinimapDelegate(const TDelegate<void(UObject*, const FMapCoord&, uint8, uint8)>&);
      #pragma endregion FSmithMapObjOperator Interface
      // end of FSmithMapObjOperator Interface
      private:
        ///
        ///	@brief FSmithMapObjOperator実装クラス(pImplイディオム)
        ///
        class MapObjOperatorImpl;
        TUniquePtr<MapObjOperatorImpl> m_pImpl;
    };
  }
}

#endif
