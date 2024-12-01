// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithCommandFormat.h"

// Should always be the last include
#include "SmithStaticObj.generated.h"


UCLASS()
class SMITH_API ASmithStaticObj : public AActor
{
	GENERATED_BODY()
//---------------------------------------
/*
							ctor(UObject)
*/
//---------------------------------------
public:	
	// Sets default values for this actor's properties
	ASmithStaticObj();

//---------------------------------------
/*
					   UObject Livecycle
*/
//---------------------------------------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//---------------------------------------
/*
							UProperty変数
*/
//---------------------------------------
public:

private:
	UPROPERTY(EditInstanceOnly)
	FMapCoord m_mapCoord;

//---------------------------------------
/*
							　　変数
*/
//---------------------------------------
private:
	UE::Smith::Battle::FSmithCommandFormat m_format;
};
