// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

#include "SmithInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FSmithInventoryInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventoryInfo, meta = (AllowAbstract = "true"))
	TSubclassOf<UObject> ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventoryInfo)
	int32 InventoryCapacity;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> ObjectContainer;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMITH_API USmithInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USmithInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool Insert(const FString&, UObject*);
	void Remove(const FString&, int32 idx);
	UObject* Get(const FString&, int32 idx) const;
	int32 GetQuantity(const FString&) const;
	int32 GetAll(const FString&, TArray<UObject*>& outItemList) const;
	bool ContainsCategory(const FString&) const;
	bool IsReachCapacity(const FString& category) const;

private:
	bool check_Internal(const FString&, UObject*) const;
	bool check_Internal(const FString&, int32 idx) const;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventorySettings, meta = (AllowPrivateAccess = "true"))
	TMap<FString,FSmithInventoryInfo> InventoryContainers;
};
