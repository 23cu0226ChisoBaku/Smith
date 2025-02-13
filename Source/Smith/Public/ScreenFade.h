// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenFade.generated.h"

enum FadeStatus	: uint8
{
	In,
	Out,
	None
};

class UBorder;
/**
 *
 */
UCLASS()
class SMITH_API UScreenFade final: public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);	

public:
	TMulticastDelegate<void()> OnFadeInStartEvent;
	TMulticastDelegate<void()> OnFadeInEndEvent;
	TMulticastDelegate<void()> OnFadeOutStartEvent;
	TMulticastDelegate<void()> OnFadeOutEndEvent;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();

public:
	void StartFade(FadeStatus);

public:
	bool IsFinish();

private:
	UPROPERTY(meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UBorder *FadeBorder;
	UPROPERTY(meta = (AllowPrivateAccess = "true"), BluePrintReadWrite)
	bool m_animFinish;
	UPROPERTY(meta = (AllowPrivateAccess = "true"), BluePrintReadOnly)
	UUserWidget *m_userWidget;

private:
	FadeStatus m_status;
};
