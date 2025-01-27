// Fill out your copyright notice in the Description page of Project Settings.


#include "HPUIComponent.h"
#include "Components/ProgressBar.h"
#include "SmithPlayerHP.h"
#include "Kismet/GameplayStatics.h"
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

void UHPUIComponent::CreateHP(APlayerController* playerCtrl)
{
	if (m_playerHPWidget != nullptr)
	{
		return;
	}

	if (!::IsValid(playerCtrl))
	{
		return;
	}

	AActor* owner = GetOwner();
	if (owner == StaticCast<AActor*>(playerCtrl->GetPawn()))
	{
		if (HPWidgetSubclass == nullptr)
		{
			HPWidgetSubclass = TSoftClassPtr<UUserWidget>(FSoftObjectPath("/Game/TestLevel/BP/BP_SmithPlayerHP.BP_SmithPlayerHP_C")).LoadSynchronous();
		}

		m_playerHPWidget = CreateWidget<USmithPlayerHP>(GetWorld(), HPWidgetSubclass, TEXT("Player HP UI"));
		check(m_playerHPWidget != nullptr);

		m_playerHPWidget->AddToViewport();
	}
	
}

void UHPUIComponent::SetHP(float percentage)
{
	if(m_playerHPWidget != nullptr)
	{
		m_playerHPWidget->SetHP(percentage);
	}
}

void UHPUIComponent::SetWidgetVisibility(bool bIsVisible)
{
	if (m_playerHPWidget == nullptr)
	{
		return;
	}

	ESlateVisibility visibility = bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	m_playerHPWidget->SetVisibility(visibility);
}

