// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithInventoryComponent.h"
#include "MLibrary.h"

// Sets default values for this component's properties
USmithInventoryComponent::USmithInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USmithInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	for (auto& inventory : InventoryContainers)
	{
		check(inventory.Value.ObjectType != nullptr);
		if (inventory.Value.InventoryCapacity > 0)
		{
			inventory.Value.ObjectContainer.Reserve(inventory.Value.InventoryCapacity);
		}
	}
}

// Called every frame
// 使用しない
void USmithInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	return;
	// ...
}

bool USmithInventoryComponent::Insert(const FString& inventoryCategoryName, UObject* itemObject)
{
	if (!check_Internal(inventoryCategoryName, itemObject))
	{
		MDebug::LogError("Failed To Insert");
		return false;
	}

	if (InventoryContainers[inventoryCategoryName].ObjectContainer.Num() >= InventoryContainers[inventoryCategoryName].InventoryCapacity)
	{
		MDebug::LogError("Can not insert item to inventory. --- inventory full");
		return false;
	}

	// Outerを変更
	itemObject->Rename(nullptr, GetOwner());
	InventoryContainers[inventoryCategoryName].ObjectContainer.Emplace(itemObject);
	return true;
}

void USmithInventoryComponent::Remove(const FString& inventoryCategoryName, int32 idx)
{
	if (!check_Internal(inventoryCategoryName, idx))
	{
		MDebug::LogError("Failed To Remove");
		return;
	}

	UObject* item = InventoryContainers[inventoryCategoryName].ObjectContainer[idx];
	if (item != nullptr && item->IsValidLowLevel())
	{
		item->ConditionalBeginDestroy();
	}

	InventoryContainers[inventoryCategoryName].ObjectContainer.RemoveAt(idx);
}

UObject* USmithInventoryComponent::Get(const FString& inventoryCategoryName, int32 containerIdx) const
{
	if (!check_Internal(inventoryCategoryName, containerIdx))
	{
		MDebug::LogError("Unable to get object, return nullptr");
		return nullptr;
	}
	
	return InventoryContainers[inventoryCategoryName].ObjectContainer[containerIdx];
}

int32 USmithInventoryComponent::GetQuantity(const FString& inventoryCategoryName) const
{
	if (!InventoryContainers.Contains(inventoryCategoryName))
	{
		return -1;
	}

	return InventoryContainers[inventoryCategoryName].ObjectContainer.Num();
}

bool USmithInventoryComponent::check_Internal(const FString& inventoryCategoryName, UObject* itemObject) const
{
	if (!InventoryContainers.Contains(inventoryCategoryName))
	{
		MDebug::LogError("Inventory category name not exist");
		return false;
	}

	if (!::IsValid(itemObject))
	{
		MDebug::LogError("Item is invalid");
		return false;
	}

	const FSmithInventoryInfo& foundInventory = InventoryContainers[inventoryCategoryName];

	if (!(itemObject->IsA(foundInventory.ObjectType)))
	{
		MDebug::Log("Is not child of category type:" + inventoryCategoryName);
		return false;
	}

	return true;
}

int32 USmithInventoryComponent::GetAll(const FString& categoryName, TArray<UObject*>& outItemList) const
{
	outItemList.Reset();
	if (InventoryContainers.Contains(categoryName))
	{
		for(const auto& item : InventoryContainers[categoryName].ObjectContainer)
		{
			outItemList.Emplace(item);
		}
	}
	return outItemList.Num();
}


bool USmithInventoryComponent::check_Internal(const FString& inventoryCategoryName, int32 idx) const
{
	if (!InventoryContainers.Contains(inventoryCategoryName))
	{
		MDebug::LogError("Inventory category name not exist");
		return false;
	}

	const FSmithInventoryInfo& foundInventory = InventoryContainers[inventoryCategoryName];

	if (idx < 0 || foundInventory.ObjectContainer.Num() <= idx)
	{
		MDebug::LogError("Index is invalid");
		return false;
	}

	return true;
}

bool USmithInventoryComponent::ContainsCategory(const FString& category) const
{
	return InventoryContainers.Contains(category);
}

bool USmithInventoryComponent::IsReachCapacity(const FString& category) const
{
	if (!ContainsCategory(category))
	{
		return true;
	}
	const auto& inventory = InventoryContainers[category];
	return inventory.ObjectContainer.Num() >= inventory.InventoryCapacity;
}


