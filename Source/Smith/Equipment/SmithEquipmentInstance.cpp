// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithEquipmentInstance)

USmithEquipmentInstance::USmithEquipmentInstance()
  : m_ownedActor(nullptr)
{ }

/// @brief        装備の所有者(Actor)を取得
AActor* USmithEquipmentInstance::GetOwnedActor() const
{
  return m_ownedActor;
}

/// @brief        装備の所有者(Actor)を設定する
/// @param Owner  所有者
void USmithEquipmentInstance::SetOwner(AActor* Owner)
{
  m_ownedActor = Owner;
}

/// @brief 装備を装着する時に呼び出されるコールバック
void USmithEquipmentInstance::OnEquipped()
{
  onEquippedImpl();
}

/// @brief 装備を外す時に呼び出されるコールバック
void USmithEquipmentInstance::OnUnequipped()
{
  onUnequippedImpl();
}