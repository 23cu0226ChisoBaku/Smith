// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestUserWidget.generated.h"

#define VISIBLE 0	// 表示
#define HIDDEN 2	// 非表示
/**
 *
 */
UCLASS()
class SMITH_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

public:
	// 引数を渡すときはTEXT()を使用する
	void SetLogMessage(FString);
	void OutPutLog();
	
public:
	// BP側でログの表示をする（cppでは呼び出しだけ）
	UFUNCTION(BlueprintImplementableEvent)
	void OnLog_BP();

private:
	// この変数に文字列を格納してBP側で使う
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestGameLog", meta = (AllowPrivateAccess = "true"))
	FString m_outPutLog;
	// ログを非表示にするタイム
	UPROPERTY(EditAnyWhere)
	float m_visibleTime;
	// 表示するログの数
	UPROPERTY(EditAnyWhere)
	int32 m_maxVisibleQuantity;

private:
	// ログを保存しておく
	TArray<FString> m_logArray;
	float m_timer;
	bool m_isVisibility;
};
