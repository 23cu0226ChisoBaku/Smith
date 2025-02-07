// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include "CoreMinimal.h"
// #include "GameFramework/Pawn.h"
// #include "TestLog.generated.h"

// class UUserWidget;
// class UGameLogWidget;
// class URichTextBlock;

// UCLASS()
// class SMITH_API ATestLog : public APawn
// {
// 	GENERATED_BODY()

// public:
// 	// Sets default values for this pawn's properties
// 	ATestLog();

// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;

// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

// 	// Called to bind functionality to input
// 	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// public:
// 	// BP側でログの表示をする（cppでは呼び出しだけ）
// 	UFUNCTION(BluePrintCallable)
// 	void TestOutPut_BP(FString str);

// private:
// 	UPROPERTY()
// 	TObjectPtr<UGameLogWidget> m_widget;
// 	TSubclassOf<UUserWidget> m_widgetClass;
// };
