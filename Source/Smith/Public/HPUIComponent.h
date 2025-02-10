// Fill out your copyright notice in the Description page of Project Settings.
/*

HPUIComponent.h

Author : MAI ZHICONG(バクチソウ)

Description : HPを表示するコンポネント

Update History: 2025/01/25 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPUIComponent.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
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
	///
	///	@brief                    HPバーを作成
	///	@param APlayerController  作成するプレイヤーのコントローラー
	///
	void CreateHP(APlayerController*);
	///
	/// @brief                    HPバーの割合を設定
	/// @param percentage         HPの割合
	///
	void SetHP(float percentage);
	///
	///	@brief                    HPの数値を設定する
	///	@param maxHp					    最大HP
	/// @param currentHp					現在のHP
	///
	void SetHPNumber(int32 maxHp,int32 currentHp);
  ///
  /// @brief                    HPバーの可視性設定
  /// @param bIsVisible         見えるようにする    ->  true
  ///                           見えないようにする  ->  false
  ///
	void SetWidgetVisibility(bool bIsVisible);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USmithPlayerHP> HPWidgetSubclass;
	UPROPERTY()
	TObjectPtr<USmithPlayerHP> m_playerHPWidget;		
};
