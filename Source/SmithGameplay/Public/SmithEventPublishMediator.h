// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IEventPublishMediator.h"
#include "SmithEventPublishMediator.generated.h"

class USmithEventPublisher;
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapSystem;
	}
}
/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithEventPublishMediator : public UObject, public IEventPublishMediator
{
	GENERATED_BODY()

public:
	USmithEventPublishMediator(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	void Initialize(USmithEventPublisher*, TSharedPtr<UE::Smith::Map::FSmithMapSystem>);

public:
	virtual void PublishPickUpEvent(AActor* Requester, IPickable*) override;

private:
	TWeakObjectPtr<USmithEventPublisher> m_eventPublisher;
	TWeakPtr<UE::Smith::Map::FSmithMapSystem> m_mapSys;
};
