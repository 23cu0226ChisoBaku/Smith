// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithUpgradeItemHandle.h"

FSmithUpgradeItemHandle::FSmithUpgradeItemHandle()
  : IconImage2D(nullptr)
  , Name{}
  , Description{}
  , Param{}
{}

FSmithUpgradeItemHandle::FSmithUpgradeItemHandle(UTexture2D* icon, const FString& name, const FString& description, const FParams& param)
  : IconImage2D(icon)
  , Name(name)
  , Description(description)
  , Param(param)
{}