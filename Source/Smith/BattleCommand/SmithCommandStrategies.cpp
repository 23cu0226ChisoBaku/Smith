// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithCommandStrategies.h"

#include "GameFramework/Actor.h"
#include "ISmithDamageApplicant.h"
#include "SmithBattleLogWorldSubsystem.h"

void SmithDefaultDamageStrategy::operator()()
{
  if (DamageApplicant != nullptr)
  {
    DamageApplicant->ApplyDamage(Instigator, Causer, ::MoveTemp(Handle), FromDirection);
  }
}

void SmithDefaultMoveStrategy::operator()(float DeltaTime)
{
  check(MoveEntity != nullptr);

	const FVector curtLocation = MoveEntity->GetActorLocation();
	const FVector leftDistanceVec = Destination - curtLocation;
	FVector curtMovement = leftDistanceVec.GetSafeNormal() * DeltaTime * MoveSpeed;

  // 移動ベクトルが残りベクトルを超えないように
	if (curtMovement.SquaredLength() > leftDistanceVec.SquaredLength())
	{
		curtMovement = leftDistanceVec;
	}

	const FVector nextLocation = curtLocation + curtMovement;
	MoveEntity->SetActorLocation(nextLocation);
}

//
SmithDefaultMoveStrategy::operator bool() const noexcept
{
  if (MoveEntity == nullptr)
  {
    return true;
  }

  return MoveEntity->GetActorLocation().Equals(Destination);
}
