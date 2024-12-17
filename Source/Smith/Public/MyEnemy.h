// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h" // �ǉ�
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"// �ǉ�
#include "DrawDebugHelpers.h"  // DrawDebugLine��DrawDebugPoint�Ȃǂ��g�p���邽�߂ɕK�v


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

	UPROPERTY(EditAnywhere)
	int32 HP;
	UPROPERTY(EditAnywhere)
	int32 ATK;

private:
	void PlayerCheck();

	UActorComponent* m_moveComp;
	UActorComponent* m_attackComp;

	float m_timer;
};
