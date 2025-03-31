// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.h

Author : MAI ZHICONG(バクチソウ)

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/08 作成
                2025/01/20 イベント配置インターフェース追加
                2025/01/21 イベントの回転パラメーターを追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_DEPLOY_DIRECTOR
#define SMITH_MAP_DEPLOY_DIRECTOR

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ISmithMapEvent;
class UObject;
struct FMapCoord;

namespace UE::Smith
{
  namespace Map
  {
    //---------------------------------------
    /*
                      前方宣言
    */
    //---------------------------------------
    struct FSmithMapDataModel;

    ///
    ///	@brief マップにものを新しく配置するクラス
    /// namespace UE::Smith::Map
    ///
    class FSmithMapDeployDirector
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapDeployDirector();
        ~FSmithMapDeployDirector();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapDeployDirector(FSmithMapDeployDirector&&) noexcept;
        FSmithMapDeployDirector& operator=(FSmithMapDeployDirector&&) noexcept;

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithMapDeployDirector(const FSmithMapDeployDirector&) = delete;
        FSmithMapDeployDirector& operator=(const FSmithMapDeployDirector&) = delete;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithMapDeployDirector Interface
      #pragma region FSmithMapDeployDirector Interface
      public:
        /// 
        /// @brief                          マップモデルを登録する
        /// @param FSmithMapDataModel       マップモデル
        ///
        void AssignMapDataModel(TSharedPtr<FSmithMapDataModel>);
        ///
        /// @brief  オブジェクトを配置する
        /// @param  AActor                  マップオブジェクト
        /// @param  x	                      X座標
        /// @param  y	                      Y座標
        ///
        void DeployMapObj(AActor*, uint8 x, uint8 y);
        ///
        /// @brief  イベントを配置する
        /// @param  ISmithMapEvent          イベントオブジェクト
        /// @param  x	                      X座標
        /// @param  y	                      Y座標
        ///
        void DeployEvent(ISmithMapEvent*, uint8 x, uint8 y);
        void AddUpdateMinimapDelegate(const TDelegate<void(UObject*, const FMapCoord&, uint8, uint8)>&);
      #pragma endregion FSmithMapDeployDirector Interface
      // end of FSmithMapDeployDirector Interface
      private:
        ///
        /// @brief FSmithMapDeployDirector(pImplイディオム)
        ///
        class DeployDirectorImpl;
        TUniquePtr<DeployDirectorImpl> m_pImpl;
    };
  }
}

#endif
