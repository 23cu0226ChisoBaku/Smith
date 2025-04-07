// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnhanceableEQInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithEnhanceableEQInstance)

void USmithEnhanceableEQInstance::Enhance(IParamAbsorbable*)
{}

void USmithEnhanceableEQInstance::OnEnhanced()
{
  onEnhancedImpl();
}