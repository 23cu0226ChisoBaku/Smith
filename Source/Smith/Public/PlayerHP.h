// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerHP.generated.h"

class UUserWidget;
class UHPUIComponent;
/**
 *
 */
UCLASS()
class SMITH_API APlayerHP : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerHP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "TestPlayerHP")
	void OnHP();

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> m_widget;
	UPROPERTY(BlueprintReadOnly)
	float m_percent;

private:
	UPROPERTY()
	TObjectPtr<UHPUIComponent> m_hpComp;

private:
	int32 m_maxHp;
	int32 m_currentHp;
};
