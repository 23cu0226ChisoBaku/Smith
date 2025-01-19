// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LogController.generated.h"

class UUserWidget;

UCLASS()
class SMITH_API ALogController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALogController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TSubclassOf<UUserWidget> m_widgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> m_widget;

private:
	float timer;
	int32 cnt;
};
