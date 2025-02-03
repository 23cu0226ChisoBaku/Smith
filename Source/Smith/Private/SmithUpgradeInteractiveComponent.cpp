// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeInteractiveComponent.h"
#include "SmithUpgradeWidget.h"

#include "SmithWeapon.h"
#include "SmithUpgradeItemHandle.h"

#include "MLibrary.h"


// Sets default values for this component's properties
USmithUpgradeInteractiveComponent::USmithUpgradeInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USmithUpgradeInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UpgradeWidgetSubclass != nullptr)
	{
		m_upgradeWidget = CreateWidget<USmithUpgradeWidget>(GetWorld(), UpgradeWidgetSubclass);
	}

	if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->AddToViewport();
		m_upgradeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


// Called every frame
void USmithUpgradeInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USmithUpgradeInteractiveComponent::ActivateUpgradeMenu()
{
	if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USmithUpgradeInteractiveComponent::DeactivateUpgradeMenu()
{
	if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->ResetWidget();
		m_upgradeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmithUpgradeInteractiveComponent::SetWeaponInfo(FSmithWeaponInfoHandle handle)
{
	if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->SetWeaponInfo(handle.Name, handle.Level, handle.Image2D);
		m_upgradeWidget->SetWeaponParam(handle.Param);
	}
}

void USmithUpgradeInteractiveComponent::SetUpgradeItems(const TArray<UObject*>& itemHandles)
{
	if (m_upgradeWidget != nullptr)
	{
		m_upgradeWidget->SetUpgradeEntryItems(itemHandles);
	}
}

void USmithUpgradeInteractiveComponent::SelectNextItem(ESelectDirection direction)
{
	if (m_upgradeWidget == nullptr || !m_upgradeWidget->IsVisible())
	{
		return;
	}

	switch(direction)
	{
		case ESelectDirection::Up:
		{
			m_upgradeWidget->SelectUp();
		}
		break;
		case ESelectDirection::Down:
		{
			m_upgradeWidget->SelectDown();
		}
		break;
	}
}

int32 USmithUpgradeInteractiveComponent::GetSelectingItemIdx() const
{
	return m_upgradeWidget != nullptr ? m_upgradeWidget->GetSelectingItemIdx() : -1;
}
