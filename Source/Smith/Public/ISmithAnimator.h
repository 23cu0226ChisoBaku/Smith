// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithAnimator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ISmithAnimator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SwitchAnimation(uint8 animationState) = 0;
	virtual void SwitchAnimationDelay(uint8 animationState, float delay) = 0;
	virtual void UpdateAnimation(float deltaTime) = 0;
	virtual bool IsAnimationFinish() const = 0;
};
