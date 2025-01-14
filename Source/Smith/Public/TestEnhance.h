// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IEnhanceable.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SMITH_API TestEnhance final:public IEnhanceable
{
public:
	TestEnhance();
	~TestEnhance();

	public:
	void Enhance() override final;
	void SetEnhance(int32,int32) override final;

	private:
	int32 m_wepon;
	int32 m_item;
};
