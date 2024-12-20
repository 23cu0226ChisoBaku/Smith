// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAttackable.h"
#include "TestTileMove_AttackableObj.generated.h"

UCLASS()
class SMITH_API ATestTileMove_AttackableObj : public AActor , public IAttackable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestTileMove_AttackableObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void OnAttack(AttackHandle&& attack) override;

};
