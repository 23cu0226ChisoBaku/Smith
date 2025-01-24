// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "TestEnhanceActor.generated.h"

// class IEnhanceable;
// class IEnhanceSystem;

// struct FParams;

// UCLASS()
// class SMITH_API ATestEnhanceActor : public AActor
// {
// 	GENERATED_BODY()

// public:
// 	// Sets default values for this actor's properties
// 	ATestEnhanceActor();

// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;

// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

// private:
// 	void EnhanceAction(FParams);

// private:
// 	TSharedPtr<IEnhanceSystem> m_enhanceSystem;
// 	TSharedPtr<IEnhanceable> m_weapon;
// };
