// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithEquipmentDefinition.h"

#include "SmithEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithEquipmentDefinition)

USmithEquipmentDefinition::USmithEquipmentDefinition()
{
  InstanceType = USmithEquipmentInstance::StaticClass();
  Data.ImageTexture = nullptr;
}