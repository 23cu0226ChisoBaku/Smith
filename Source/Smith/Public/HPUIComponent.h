// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPUIComponent.generated.h"

class USmithPlayerHP;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMITH_API UHPUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHPUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void CreateHP(APlayerController*);
	void SetHP(float percentage);
	void SetHPNumber(int32,int32);
	void SetWidgetVisibility(bool bIsVisible);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USmithPlayerHP> HPWidgetSubclass;
	UPROPERTY()
	TObjectPtr<USmithPlayerHP> m_playerHPWidget;		
};
