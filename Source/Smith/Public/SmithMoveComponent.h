
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
	virtual void Move(float deltaTime) override;
	virtual void SetDestination(FVector) override;
	virtual bool IsReachDestination() const override;
	void SetTerminusPos(FVector pos) ;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MoveParameter, meta = (AllowPrivateAccess = "true"))
	double MoveSpeed;

private:
	FVector m_terminus;
	uint8 m_bCanMove : 1;
};
