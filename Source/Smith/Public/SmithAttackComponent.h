// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakInterfacePtr.h"
#include "AttackHandle.h"
#include "ICanMakeAttack.h"
#include "SmithAttackComponent.generated.h"

class AMyPlayerCharacter;
class IAttackable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SMITH_API USmithAttackComponent : public UActorComponent , public ICanMakeAttack
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmithAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Attack(AMyPlayerCharacter *player, int32 damage);

	virtual void Attack() override;
	
	virtual void SetAttackTarget(IAttackable*) override;
	virtual void SetAttackHandle(AttackHandle&&) override;


private:
	TWeakInterfacePtr<IAttackable> m_attackTarget;
	AttackHandle m_attackHandle;
};
