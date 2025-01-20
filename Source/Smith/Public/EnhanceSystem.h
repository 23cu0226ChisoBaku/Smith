
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IEnhanceSystem.h"
#include "CoreMinimal.h"

class SMITH_API EnhanceSystem:public IEnhanceSystem
{
public:
  EnhanceSystem();

public:
  void Enhance(IEnhanceable*,FParams) override final;
};