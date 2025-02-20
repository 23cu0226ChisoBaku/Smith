// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithAIStrategy.h

Author : MAI ZHICONG(バクチソウ)

Description : Smith専用簡易的なAI（Behaviour Treeにする予定あり）

Update History: 2025/01/14 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SmithAIStrategy.generated.h"

/**
 * Smith AI 基底クラス
 */
UCLASS(Abstract,EditInlineNew)
class SMITH_API USmithAIStrategy : public UObject
{
  GENERATED_BODY()

//---------------------------------------
/*
                  ctor
*/
//---------------------------------------	
public:
  USmithAIStrategy(const FObjectInitializer&);
  virtual void BeginDestroy() override;

//---------------------------------------
/*
    パブリック関数(クラスインターフェース)
*/
//---------------------------------------
// USmithAIStrategy Interfaces
#pragma region USmithAIStrategy Interfaces
public:
  ///
  /// @brief          AIノード実行
  ///	@return         成功 -> true    
  ///                 失敗 -> false
  ///
  bool Execute();
  ///
  /// @brief          AIの利用者Actorを設定する
  ///	@param AActor   利用者
  ///
  void SetOwner(AActor*);
  ///
  ///	@brief          AIの利用者Actorポインタを返す
  ///	@return         有効なActor -> Actorへのポインタ　　
  ///                 無効 -> nullptr
  ///
  AActor* GetOwner() const;
#pragma endregion USmithAIStrategy Interfaces
// end of USmithAIStrategy Interfaces


private:
  ///
  /// @brief            AIノード実行実装関数（子クラスをオーバーライドすること）
  ///	@return           成功 -> true    
  ///                   失敗 -> false
  ///
  virtual bool executeImpl() PURE_VIRTUAL(USmithAIStrategy::executeImpl, return false;);

private:
  ///
  /// @brief            AI利用者ポインタ
  ///
  UPROPERTY()
  TObjectPtr<AActor> m_owner;
};
