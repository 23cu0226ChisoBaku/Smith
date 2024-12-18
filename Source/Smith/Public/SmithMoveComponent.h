
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IMoveable.h"
#include "SmithMoveComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class SMITH_API USmithMoveComponent : public UActorComponent , public IMoveable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	USmithMoveComponent();

public:
	virtual void Move() override;
	void SetMoveSpeed(float speed)
	{
		MOVE_DISTANCE = speed;
	}
public:
	UPROPERTY(EditAnywhere)
	float MOVE_DISTANCE;

private:


	AActor *m_target;
	AActor *m_enemyObj;
	FVector m_myPos;
	FVector m_targetPos;

	// 仮のタイマー
	float m_temporaryTimer;
};
