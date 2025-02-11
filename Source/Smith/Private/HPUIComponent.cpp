// Fill out your copyright notice in the Description page of Project Settings.
/*

HPUIComponent.cpp

Author : MAI ZHICONG(バクチソウ)

Description : HPを表示するコンポネント

Update History: 2025/01/25 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "HPUIComponent.h"
#include "SmithPlayerHP.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHPUIComponent::UHPUIComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHPUIComponent::BeginPlay()
{
  Super::BeginPlay();
}


// Called every frame
void UHPUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
      // TODO
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

void UHPUIComponent::SetHPNumber(int32 maxHp,int32 currentHp)
{
	if(m_playerHPWidget != nullptr)
	{
		m_playerHPWidget->SetHPNumber(maxHp,currentHp);
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

