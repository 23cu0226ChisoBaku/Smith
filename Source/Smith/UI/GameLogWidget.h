// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLogWidget.generated.h"

#define VISIBLE 0 // 表示
#define HIDDEN 2	// 非表示

class URichTextBlock;
class UBorder;
/**
 *
 */
UCLASS()
class SMITH_API UGameLogWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

public:
	// 引数を渡すときはTEXT()を使用する
	void AddLogMessage(FString);
	void OutputLog();

public:
	// BP側でログの表示をする（cppでは呼び出しだけ）
	UFUNCTION(BlueprintImplementableEvent)
	void OnLog_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void AddLog_BP();
	// ログが更新されたときの処理
	UFUNCTION(BluePrintCallable,Category = "GameLog")
	void UpdatedLog();

public:
	// BP側の変数とバインド（処理が終わっているのでプロジェクトが終わったら改善する）
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	URichTextBlock* RichText1;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	URichTextBlock* RichText2;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	URichTextBlock* RichText3;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	URichTextBlock* RichText4;

	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	UBorder* Border1;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	UBorder* Border2;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	UBorder* Border3;
	UPROPERTY(BluePrintReadWrite,meta = (BindWidget))
	UBorder* Border4;

private:
	// この変数に文字列を格納してBP側で使う
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestGameLog", meta = (AllowPrivateAccess = "true"))
	TArray<FString> m_outputLogArr;
	// 超過した分保存しておく
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestGameLog", meta = (AllowPrivateAccess = "true"))
	TArray<FString> m_logStorage;
	// ログを非表示にするタイム
	UPROPERTY(EditAnyWhere)
	float m_visibleTime;
	// 表示するログの数
	UPROPERTY(EditAnyWhere,BluePrintReadOnly,meta = (AllowPrivateAccess = "true") )
	int32 m_maxVisibleQuantity;
	// アルファを下げるスピード
	UPROPERTY(EditAnyWhere)
	float m_alphaSubtractionSpeed;

private:
	TArray<UBorder*> m_borderArr;
	FLinearColor m_currentAlpha;
	float m_timer;
	bool m_isVisibility;
};
