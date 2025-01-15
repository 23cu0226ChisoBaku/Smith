// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBTTaskNode_SmithTestNode.h"
#include "MLibrary.h"

USmithBTTaskNode_SmithTestNode::USmithBTTaskNode_SmithTestNode(const FObjectInitializer& ObjInit)
  : Super(ObjInit)
{
  NodeName = TEXT("Smith BT Task Node Test");
  INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type USmithBTTaskNode_SmithTestNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  MDebug::Log("Self Task Execute");
  return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type USmithBTTaskNode_SmithTestNode::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  return Super::AbortTask(OwnerComp, NodeMemory);
}

void USmithBTTaskNode_SmithTestNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void USmithBTTaskNode_SmithTestNode::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
  Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

FString USmithBTTaskNode_SmithTestNode::GetStaticDescription() const
{
  if (TestFloat > 0.0f)
  {
    MDebug::LogWarning("More than 0.0f");
    return FString::Printf(TEXT("Blackboard Key Is: %s%sTestFloatValue is: %s"), *BlackboardKey.SelectedKeyName.ToString(), LINE_TERMINATOR, *FString::SanitizeFloat(TestFloat));
  }
  else
  {
    return FString::Printf(TEXT("Blackboard Key Is: %s"), *BlackboardKey.SelectedKeyName.ToString());
  }
}

#if WITH_EDITOR
  FName USmithBTTaskNode_SmithTestNode::GetNodeIconName() const
  {
    return FName("BTEditor.Graph.BTNode.Task.MoveTo.Icon");
  }
#endif // WITH_EDITOR