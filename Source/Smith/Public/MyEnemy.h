// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h" // 追加
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"// 追加
#include "DrawDebugHelpers.h"  // DrawDebugLineやDrawDebugPointなどを使用するために必要


#include "MyEnemy.generated.h"



UCLASS()
class SMITH_API AMyEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere)
	//AActor Player;

private:
	void Attack();
};
