
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmithMoveComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SMITH_API USmithMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USmithMoveComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
	float MOVE_DISTANCE;

private:
	void Move();

	AActor* m_target;
	AActor* m_enemyObj;
	FVector m_myPos;
	FVector m_targetPos;

	// 仮のタイマー
	float m_temporaryTimer;
};
