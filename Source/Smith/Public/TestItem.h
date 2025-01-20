// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IItem.h"

/**
 *
 */
class SMITH_API TestItem : public IItem
{
public:
	TestItem(FString,int32,int32,int32,int32);
	~TestItem();

public:
	FParams GetParam() override;
	FString GetItemName() override; 
private:
	FParams m_param;
	FString m_name;
};
