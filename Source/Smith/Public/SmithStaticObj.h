// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dimension2Array.h"

// Should always be the last include
#include "SmithStaticObj.generated.h"


UCLASS()
class SMITH_API ASmithStaticObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmithStaticObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UE::MLibrary::TDimension2Array<int32> m_arr;

};
