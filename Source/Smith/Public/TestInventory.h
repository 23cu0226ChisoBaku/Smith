// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class TestItem;
struct FParams;
/**
 *
 */
class SMITH_API TestInventory
{
public:
	TestInventory();
	~TestInventory();

public:
	void SetItem(TestItem);
	FParams GetItemParam(int32);
	FString GetItemName(int32);
	int32 ItemNum();

private:
	TArray<TestItem> m_items;
};
