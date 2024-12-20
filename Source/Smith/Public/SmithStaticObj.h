// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithCommandFormat.h"
#include "IAttackable.h"
#include "ITurnManageable.h"
// Should always be the last include
#include "SmithStaticObj.generated.h"


UCLASS()
class SMITH_API ASmithStaticObj : public AActor, public IAttackable
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

public:
	void OnAttack(AttackHandle&& attack) override;
//---------------------------------------
/*
						Privateメソッド
*/
//---------------------------------------
private:

//---------------------------------------
/*
							UProperty変数
*/
//---------------------------------------
public:

private:
	UPROPERTY(EditInstanceOnly)
	FMapCoord m_mapCoord;

	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<UDataTable> m_dataTable;

	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<UDataTable> m_dataTableDiagonal;

	UPROPERTY()
	UDataTable* m_dataRef;

	UPROPERTY()
	UDataTable* m_dataDiagonalRef;
//---------------------------------------
/*
							　　変数
*/
//---------------------------------------
private:
	UE::Smith::Battle::FSmithCommandFormat m_format;
};
