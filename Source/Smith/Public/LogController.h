// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LogController.generated.h"

class UUserWidget;
class UGameLogWidget;
class GameLogMediator;
class ISendableLog;

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
	TObjectPtr<UGameLogWidget> m_widget;
	TSharedPtr<ISendableLog> m_testSend;
	TSharedPtr<GameLogMediator> m_mediator;

private:
	float timer;
	int32 cnt;
};
