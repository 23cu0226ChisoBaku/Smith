// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "SmithEquipmentInstance.generated.h"

UCLASS(Abstract)
class SMITH_API USmithEquipmentInstance : public UObject
{
  GENERATED_BODY()

public:

  USmithEquipmentInstance();

  /// @brief        装備の所有者(Actor)を取得
  AActor* GetOwnedActor() const;

  /// @brief        装備の所有者(ActorType)を取得
  /// template ActorType(must be AActor or child class)
  template<typename ActorType>
  ActorType* GetOwnedTypeActor() const;

  /// @brief        装備の所有者(Actor)を設定する
  /// @param Owner  所有者
  void SetOwner(AActor* Owner);

  //--------------------------------------------
  // virtual method of USmithEquipmentInstance
  //--------------------------------------------
  /// @brief 装備を装着する時に呼び出されるコールバック
  void OnEquipped();

  /// @brief 装備を外す時に呼び出されるコールバック
  void OnUnequipped();

private:

  /// @brief 装備を装着する実装メソッド
  virtual void onEquippedImpl() PURE_VIRTUAL(ThisClass::onEquippedImpl,);

  /// @brief 装備を外す実装メソッド
  virtual void onUnequippedImpl() PURE_VIRTUAL(ThisClass::onUnequippedImpl,);

private:

  TObjectPtr<AActor> m_ownedActor;

};

/// @brief        装備の所有者(ActorType)を取得
/// template ActorType(must be AActor or child class)
template<typename ActorType>
ActorType* USmithEquipmentInstance::GetOwnedTypeActor() const
{
  return CastChecked<ActorType>(m_ownedActor);
}
