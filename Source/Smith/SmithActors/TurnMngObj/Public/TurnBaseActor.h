// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ITurnManageable.h"
#include "ICanCommandMediate.h"
#include "IMinimapDisplayable.h"

#include "UObject/WeakInterfacePtr.h"

#include "TurnBaseActor.generated.h"

class USmithAIStrategy;
class USmithAIBehaviorProcessor;

UCLASS(Abstract)
class SMITH_API ATurnBaseActor : public AActor , public ITurnManageable
                               , public ICanCommandMediate, public IMinimapDisplayable
{
  GENERATED_BODY()
  
public:	
  // Sets default values for this actor's properties
  ATurnBaseActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay();

public:	
  // Called every frame
  virtual void Tick(float DeltaTime);

public:

  // Interfaces
  #pragma region Interfaces
    // ICanCommandMediate
    #pragma region ICanCommandMediate
    void SetCommandMediator(ICommandMediator*) override final;
    #pragma endregion ICanCommandMediate
    // end of ICanCommandMediate

    virtual void InitializeParameter(int32 currentLevel);

    UTexture2D* GetMinimapDisplayTexture_Implementation() override final;
  #pragma endregion Interfaces
  // end of Interfaces

private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
  bool bUseSmithAIProcessor;
  UPROPERTY(EditAnywhere)
  TObjectPtr<UTexture2D> MinimapTexture;

protected:
  UPROPERTY()
  TObjectPtr<USmithAIBehaviorProcessor> m_aiBehaviorProcessor;

protected:
  TWeakInterfacePtr<ICommandMediator> m_commandMediator;

public:
  TMulticastDelegate<void()> OnDefeatEvent;
  
};
