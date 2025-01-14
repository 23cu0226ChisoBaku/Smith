
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IEnhanceable.h"
#include "CoreMinimal.h"

class SMITH_API EnhanceSystem:public IEnhanceable
{
public:
  EnhanceSystem();

public:
  void Enhance(int32,int32) override final;
};