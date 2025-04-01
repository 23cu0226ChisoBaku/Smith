// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "ISmithAnimator.generated.h"

namespace UE::Smith
{
  enum EAnimationState : uint8
  {
    SMITH_ANIM_IDLE = 0,
    SMITH_ANIM_WALK = 1,
    SMITH_ANIM_ATTACK = 2,
    SMITH_ANIM_DAMAGED = 3,
    SMITH_ANIM_DEAD = 4,
    SMITH_ANIM_SKILL_ONE = 5,
    SMITH_ANIM_SKILL_TWO = 6,
    SMITH_ANIM_SKILL_THREE = 7,
    
    SMITH_ANIM_STATE_CNT,
  };
}

UINTERFACE(MinimalAPI)
class USmithAnimator : public UInterface
{
  GENERATED_BODY()
};


class SMITH_API ISmithAnimator
{
  GENERATED_BODY()

public:
  ///
  /// @brief                    アニメーションを切り替える（遅延なし）
  /// @param animationState     アニメーションステート(基本UE::Smith::EAnimationState使用)
  ///
  virtual void SwitchAnimation(uint8 animationState) = 0;
  ///
  /// @brief                    アニメーションを切り替える（遅延あり）
  /// @param animationState     アニメーションステート(基本UE::Smith::EAnimationState使用)
  /// @param delay              遅延時間
  ///
  virtual void SwitchAnimationDelay(uint8 animationState, float delay) { SwitchAnimation(animationState);}
  ///
  /// @brief                    アニメーション時間を更新する
  /// @param deltaTime          デルタタイム
  ///
  virtual void UpdateAnimation(float deltaTime) = 0;
  ///
  /// @brief                    更新した後にアニメーション再生が終了したかを調べる（TODO：修正する予定）
  /// @return
  ///
  virtual bool IsAnimationFinish() const = 0;
};
