// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithDangerZoneDisplayer.generated.h"

UCLASS()
class SMITH_API ASmithDangerZoneDisplayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmithDangerZoneDisplayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetupDisplayLocations(const TArray<FVector>&);
	void Dispose();

private:
	// 攻撃範囲を表示する所のプレイン
	UPROPERTY()
	TArray<AActor*> m_dangerZonePlane;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlaneObjectSubclass;
};
