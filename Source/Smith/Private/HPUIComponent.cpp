// Fill out your copyright notice in the Description page of Project Settings.


#include "HPUIComponent.h"
#include "Components/ProgressBar.h"
#include "SmithPlayerHP.h"
#include "Debug.h"

// Sets default values for this component's properties
UHPUIComponent::UHPUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHPUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHPUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHPUIComponent::SetUIHP(UUserWidget* widget,float percent)
{
	USmithPlayerHP* hp = Cast<USmithPlayerHP>(widget);
	if(hp != nullptr)
	{
		hp->SetHP(percent);
		MDebug::LogError(TEXT("キャスト成功"));
	}
	else
	{
		MDebug::LogError(TEXT("キャスト失敗"));
	}
}

