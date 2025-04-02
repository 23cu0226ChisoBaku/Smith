// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapManager.h

Author : MAI ZHICONG(バクチソウ)

Description : マップマネージャー

Update History: 2025/01/04 作成
              : 2025/01/06 クラス分けてマップを操作する

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_SYSTEM
#define SMITH_MAP_SYSTEM

//---------------------------------------
/*
									前方宣言
*/
//---------------------------------------
class IEventRegister;
class ISmithMapEvent;
class IMinimapDisplayer;
class USmithEventPublisher;
class UMinimapDisplayTypeFactory;
class ISmithMapModelRequester;
namespace UE::Smith
{
  namespace Battle
	{
    class FSmithCommandFormat;
  }
}

enum class EDirection : uint8;
enum class EDirectionPolicy : uint8;
enum class EMapDeployRule : uint8;

struct FAttackableInfoHandle;
struct FMapCoord;
struct FSmithMapBluePrint;
struct FSmithMapConstructionBluePrint;
struct FSmithEnemyGenerateBluePrint;
struct FItemGenerationListRow;
struct FSmithMapDecoration;

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

		///
		///	@brief マップシステム
		/// namespace UE::Smith::Map
		///
    class MAPMANAGEMENT_API FSmithMapSystem final : public FGCObject
    {			
      //---------------------------------------
      /*
              コンストラクタとデストラクタ
      */
      //---------------------------------------
      public:
        FSmithMapSystem();
        ~FSmithMapSystem();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapSystem(FSmithMapSystem&&) noexcept;
        FSmithMapSystem& operator=(FSmithMapSystem&&) noexcept;

      //---------------------------------------
			/*
											コピー禁止
			*/
			//---------------------------------------
      private:
        FSmithMapSystem(const FSmithMapSystem&) = delete;
        FSmithMapSystem& operator=(const FSmithMapSystem&) = delete;
      
      //---------------------------------------
			/*
							パブリック関数(インターフェース)
			*/
			//---------------------------------------
			// FSmithMapSystem Interface
			#pragma region FSmithMapSystem Interface
      public:
        ///
        /// @brief                                イベント登録インターフェースを注入
        /// @param EventRegister                  イベント登録インターフェース
        /// 
        void AssignEventRegister(IEventRegister* EventRegister);
        ///
        /// @brief                                マップモデル取得インターフェースを注入
        /// @param ModelRequester                 マップモデル取得インターフェース
        ///
        void AssignMapModelRequester(ISmithMapModelRequester* ModelRequester);
        ///
        /// @brief                                マップを初期化する
        /// @param UWorld                         ダンジョンオブジェクトを配置するUnreal Engine ワールド
        /// @param FSmithMapBluePrint             ダンジョンマップ設計図
        /// @param FSmithMapConstructionBluePrint ダンジョンマップ構築用設計図
        ///
        void InitMap(UWorld*, const FSmithMapBluePrint&, const FSmithMapConstructionBluePrint&);
        ///
        /// @brief                                マップオブジェクトを生成する
        /// @param UWorld                         マップオブジェクトを配置するUnreal Engine ワールド
        /// @param player                         プレイヤーポインタ
        /// @param FSmithEnemyGenerateBluePrint   敵生成する設計図
        ///
        void InitMapObjs(UWorld*, AActor* player, const FSmithEnemyGenerateBluePrint&);
        ///
        /// @brief                                次の階層にいくイベントを初期化
        /// @param Event                          イベントインターフェース
        ///
        void InitNextLevelEvent(ISmithMapEvent* Event);
        ///
        /// @brief                                拾いものイベントを初期化
        /// @param Rule                           配置ルール
        /// @param Events                         イベントインターフェース
        ///
        void InitPickableEvent(EMapDeployRule Rule, const TArray<ISmithMapEvent*>& Events);
        ///
        /// @brief                                ダンジョン飾りを配置する
        /// @param World                          UEワールド
        /// @param Decoration                     飾り
        ///
        void InitDecoration(UWorld* World, const FSmithMapDecoration& Decoration);
        ///
        /// @brief                                ミニマップディスプレイヤーを注入
        /// @param                                ミニマップディスプレイヤーインターフェース
        ///
        void AssignMinimapDisplayer(IMinimapDisplayer*);
        ///
        /// @brief                                ミニマップオブジェクトファクトリーを注入
        /// @param                                ミニマップオブジェクトファクトリー
        ///
        void AssignMinimapDisplayTypeFactory(UMinimapDisplayTypeFactory*);
        /// 
        /// @brief                                マップオブジェクトを配置する
        /// @param AActor                         マップオブジェクト
        /// @param x                              X座標
        /// @param y                              Y座標
        ///
        void DeployMapObj(AActor*, uint8 x, uint8 y);
        /// 
        /// @brief                                マップイベントを配置する
        /// @param Event                          マップイベント
        /// @param x                              X座標
        /// @param y                              Y座標
        ///
        void DeployEvent(ISmithMapEvent* Event, uint8 x, uint8 y);
        ///
        /// @brief                                攻撃できるオブジェクトを探す                      
        /// @param outActors                      攻撃できるオブジェクトコンテナ(OUT)
        /// @param AActor                         攻撃を行うマップオブジェクト
        /// @param FSmithCommandFormat            攻撃フォーマット
        ///
        void FindAttackableMapObjs(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor*, const UE::Smith::Battle::FSmithCommandFormat&);
        ///
        /// @brief                                攻撃できるオブジェクトを特定の座標から探す                      
        /// @param outActors                      攻撃できるオブジェクトコンテナ(OUT)
        /// @param AActor                         攻撃を行うオブジェクト
        /// @param FSmithCommandFormat            攻撃フォーマット
        /// @param offsetToLeft                   攻撃を行うオブジェクトの原点座標からXへのオフセット
        /// @param offsetToTop                    攻撃を行うオブジェクトの原点座標からYへのオフセット
        ///
        void FindAttackableMapObjsFromCoord(TArray<FAttackableInfoHandle>& outAttackableHandles, AActor*, const UE::Smith::Battle::FSmithCommandFormat&, uint8 offsetToLeft, uint8 offsetToTop);
        ///
        /// @brief                                プレイヤーと請求者の方向関係を取得
        /// @param outDirection                   方向関係
        /// @param                                方向決定ポリシー
        /// @param origin                         請求者
        /// @param offsetLeft                     請求者の原点座標からXへのオフセット
        /// @param offsetTop                      請求者の原点座標からYへのオフセット
        /// @return 
        ///
        bool GetPlayerDirection(EDirection& outDirection, EDirectionPolicy, AActor* origin, uint8 offsetLeft = 0, uint8 offsetTop = 0);
        ///
        /// @brief                                マップオブジェクトを移動
        /// @param AActor                         マップオブジェクト
        /// @param EMoveDirection                 移動方向
        /// @param moveDistance                   移動距離(何マス分)
        /// @param FVector                        移動先の座標
        ///
        void MoveMapObj(AActor*, EDirection, uint8 moveDistance, FVector&);
        ///
        /// @brief                                プレイヤーを追跡する
        /// @param outChaseDirection              プレイヤーと追跡者の方向関係
        /// @param chaser                         追跡者
        /// @param chaseRadius                    追跡半径
        ///
        bool ChasePlayerTarget(EDirection& outChaseDirection, AActor* chaser, uint8 chaseRadius);
        ///
        /// @brief                                オブジェクトの座標を取得 
        /// @param MapObj                         請求者
        /// @param outX                           取得したX座標
        /// @param outY                           取得したY座標
        /// @return                               true->見つけた false->見つからなかった(nullptrもしくは登録されていない)
        ///
        bool GetMapObjectCoord(AActor* MapObj, uint8& outX, uint8& outY);
        ///
        /// @brief                                ダンジョン座標をワールドに変換
        /// @param outLocation                    変換したいワールド座標
        /// @param x                              X座標
        /// @param y                              Y座標
        /// @return 
        ///
        bool ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y);
        ///
        /// @brief                                マップシステムをリセット
        ///
        void Reset();

			#pragma endregion FSmithMapSystem Interface
			// end of FSmithMapSystem Interface

        //---Begin of FGCObject Interface
        virtual void AddReferencedObjects(FReferenceCollector&) override;
        virtual FString GetReferencerName() const override;
        //---End of FGCObject Interface

      private:
        ///
        /// @brief マップマネージャー実装クラス(pImplイディオム)
        ///
        class MapSysImpl;
        TUniquePtr<MapSysImpl> m_pImpl;
    };
  }
}

#endif