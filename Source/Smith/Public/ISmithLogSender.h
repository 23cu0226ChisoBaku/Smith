// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "ISmithLogSender.generated.h"

class ISmithEventLogger;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithLogSender : public UInterface
{
  GENERATED_BODY()
};

class SMITH_API ISmithLogSender
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  virtual void SendAttackLog(UObject* Attacker, UObject* Defender) const = 0;
  virtual void SendDamageLog(UObject* Defender, int32 Damage) const = 0;
  virtual void SendDefeatedLog(UObject* Defeat) const = 0;
  virtual void SendInteractEventLog(UObject* Instigator, ISmithEventLogger* Event, bool bIsInteractSuccess) const = 0;
  virtual void SendEnhanceLog(UObject* EnhanceEntity) const = 0;
};
