// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmithUpgradeInteractiveComponent.generated.h"

class USmithUpgradeWidget;
struct FSmithWeaponInfoHandle;
struct FSmithUpgradeItemHandle;

enum class ESelectDirection : uint8
{
	Up = 0,
	Down = 1,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMITH_API USmithUpgradeInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USmithUpgradeInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ActivateUpgradeMenu();
	void DeactivateUpgradeMenu();
	void SetWeaponInfo(FSmithWeaponInfoHandle);
	void SetUpgradeItems(const TArray<UObject*>&);
	void SelectNextItem(ESelectDirection);
	int32 GetSelectingItemIdx() const;
private:
	UPROPERTY(EditAnywhere, Category = UpgradeWidget)
	TSubclassOf<USmithUpgradeWidget> UpgradeWidgetSubclass;
	UPROPERTY()
	TObjectPtr<USmithUpgradeWidget> m_upgradeWidget;
		
};
