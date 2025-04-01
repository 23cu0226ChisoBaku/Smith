// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventPublisher.h"

#include "ISmithMapEvent.h"
#include "SmithNextLevelEvent.h"


USmithEventPublisher::USmithEventPublisher(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithEventPublisher::BeginDestroy()
{
  Super::BeginDestroy();
}


