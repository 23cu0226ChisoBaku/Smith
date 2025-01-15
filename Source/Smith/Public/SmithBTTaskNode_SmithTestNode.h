// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "SmithBTTaskNode_SmithTestNode.generated.h"

/**
 * Smith Test Node
 * テスト専用ノード
 */
UCLASS()
class SMITH_API USmithBTTaskNode_SmithTestNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USmithBTTaskNode_SmithTestNode(const FObjectInitializer&);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector BlackboardKey;
	/**これは素晴らしい*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TestFloat;

private:
	// Event Receive Execute (Blueprint)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	// Event Receive Abort (Blueprint)
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// Event Receive Tick (Blueprint)
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	// Message handler, default implementation will finish latent execution/abortion this function should be considered as const (don't modify state of object) if node is not instanced!
	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
	// Script the message on the TaskNode visual box
	virtual FString GetStaticDescription() const override;
	
public:
#if WITH_EDITOR
	FName GetNodeIconName() const override final;
#endif // WITH_EDITOR
};
