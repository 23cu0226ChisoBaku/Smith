
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IEnhanceSystem.h"

class SMITH_API EnhanceSystem:public IEnhanceSystem
{
public:
  EnhanceSystem();

public:
  void Enhance(IEnhanceable*,IParamAbsorbable*) override final;
};