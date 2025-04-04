// Fill out your copyright notice in the Description page of Project Settings.
/*

ICommandMediator.h

Author : MAI ZHICONG(バクチソウ)

Description : コマンド仲介インタフェース

Update History: 2024/12/23 作成
                2024/12/26 全てのインタフェースにActorポインタを渡すように変更
                2025/01/07 SendMoveCommand引数追加：  UE::Smith::Battle::EMoveDirection, uint8 moveDistance
                           SendAttackCommand引数追加: UE::Smith::Battle::FSmithCommandFormat

                2025/01/14 SendAttackCommand引数追加: UE::Smith::Battle::EMoveDirection
                2025/01/16 UE::Smith::Battle::EMoveDirectionをEDirectionに変更
                2025/01/20 SendIdleCommand追加
                           SendAttackCommand引数追加: bool bAttackEventNoTarget = true

                2025/01/30 FAttackHandle引数専用SendAttackCommand関数追加
                2025/02/09 SendSkillCommand追加
                           TODO GetRangeLocations関数追加

                2025/02/13 ※リファクタリングする予定
                           TODO GetPlayerDirection関数追加

                2025/02/16 SendIdleCommand引数追加: float idleTime = 0.0f

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "UObject/Interface.h"

#include "ICommandMediator.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
struct FAttackHandle;
struct FSmithSkillParameter;
enum class EDirection : uint8;
enum class EDirectionPolicy : uint8;

namespace UE::Smith
{
  namespace Battle
  {
    class FSmithCommandFormat;
  }
}

UINTERFACE(MinimalAPI)
class UCommandMediator : public UInterface
{
  GENERATED_BODY()
};

class SMITH_API ICommandMediator
{
  GENERATED_BODY()

public:

  /// @brief                移動コマンドを送る
  /// @param                移動請求者
  /// @param                移動方向
  /// @param moveDistance   移動距離
  /// @return 
  virtual bool SendMoveCommand(AActor*, EDirection, uint8 moveDistance) = 0;

  /// @brief                待機コマンドを送る
  /// @param                待機請求者
  /// @param idleTime       待機時間
  /// @return 
  virtual bool SendIdleCommand(AActor*, float idleTime = 0.0f) = 0;

  /// @brief                      攻撃コマンドを送る
  /// @param                      攻撃請求者
  /// @param                      攻撃方向
  /// @param                      攻撃フォーマット
  /// @param                      攻撃ハンドル
  /// @param bAttackEvenNoTarget  攻撃対象がなくても攻撃を実行するかを示すフラグ
  /// @return 
  virtual bool SendAttackCommand(AActor*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true) = 0;
  
  /// @brief                      スキルコマンドを送る
  /// @param                      スキル請求者
  /// @param                      スキルパラメーター
  /// @param                      スキルフォーマット
  /// @param                      スキル攻撃ハンドル
  /// @return
  virtual bool SendSkillCommand(AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&) = 0;

  // TODO 設計を見直す
  // ダンジョン範囲内の座標を取得
  virtual int32 GetRangeLocations(TArray<FVector>& outLocations, AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&) = 0;
  // プレイヤーとの方向を取得
  virtual void GetPlayerDirection(EDirection& outDirection, EDirectionPolicy, AActor*, uint8 offsetToLeft = 0u, uint8 offsetToTop = 0u) = 0;
};
