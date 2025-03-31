// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "SmithPlayerView.generated.h"

class USmithUpgradeWidget;
class USmithPlayerHP;
class UHerbWidget;
class APlayerController;
struct FSmithWeaponInfoHandle;

UCLASS(Blueprintable, Abstract)
class SMITH_API USmithPlayerView : public UObject
{
	GENERATED_BODY()

private:
	enum class ESelectDirection : uint8
	{
		Up = 0,
		Down = 1,
	};


public:

	USmithPlayerView(const FObjectInitializer& ObjectInitializer);

	void InitPlayerView(APlayerController* PlayerController);

	//---Begin of UpgradeWidget Method
	void ShowUpgradeMenu();
	void HideUpgradeMenu();
	void InitEnhanceMenuInfo(const FSmithWeaponInfoHandle& InfoHandle, const TArray<UObject*>& ItemObjects);
	void SelectNextMenuItem(float SelectDirection);
	int32 GetSelectingItemIdx() const;
	//---End of UpgradeWidget Method
	
	//---Begin of HPWidget Method
	///
	///	@brief                    HPの数値を設定する
	/// @param CurrentHealth			現在のHP
	///	@param MaxHealth					最大HP
	///
	void SetHP(int32 CurrentHealth, int32 MaxHealth);
	//---End of HPWidget Method

	//---Begin of HerbWidget Method
	void SetHerbValue(int32 Value);
	//---End of HerbWidget Method

private:

	UPROPERTY(EditDefaultsOnly, Category = "SmithPlayer|View")
	TSubclassOf<USmithUpgradeWidget> UpgradeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "SmithPlayer|View")
	TSubclassOf<USmithPlayerHP> HPWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "SmithPlayer|View")
  TSubclassOf<UHerbWidget> HerbWidgetClass;

	UPROPERTY()
	TObjectPtr<USmithUpgradeWidget> m_upgradeWidget;

	UPROPERTY()
	TObjectPtr<USmithPlayerHP> m_hpWidget;

	UPROPERTY()
	TObjectPtr<UHerbWidget> m_herbWidget;

};
