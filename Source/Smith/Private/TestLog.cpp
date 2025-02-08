// // Fill out your copyright notice in the Description page of Project Settings.
// #include "TestLog.h"
// #include "GameLogWidget.h"
// #include "Blueprint/UserWidget.h"
// #include "Debug.h"

// // Sets default values
// ATestLog::ATestLog()
// :m_widgetClass(nullptr)
// {
//  	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = false;
// 	// 生成したいwidgetのpath
// 	FString Path = TEXT("/Game/TestLevel/BP/BP_GameLogWidget.BP_GameLogWidget_C");
// 	m_widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

// 	if (m_widgetClass == nullptr)
// 	{
// 		MDebug::LogError("Not Found widget Class");
// 	}
// }

// // Called when the game starts or when spawned
// void ATestLog::BeginPlay()
// {
// 	Super::BeginPlay();
// 	m_widget = CreateWidget<UGameLogWidget>(GetWorld(), m_widgetClass);

// 	if (m_widget == nullptr)
// 	{
// 		MDebug::LogError(TEXT("widgetがnull"));
// 		return;
// 	}

// 	m_widget->AddToViewport();
// }

// // Called every frame
// void ATestLog::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

// // Called to bind functionality to input
// void ATestLog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);

// }
// void ATestLog::TestOutPut_BP(FString str)
// {
// 	if(m_widget != nullptr)
// 	{
// 		m_widget->AddLogMessage(str);
// 		m_widget->OutputLog();
// 	}
// }