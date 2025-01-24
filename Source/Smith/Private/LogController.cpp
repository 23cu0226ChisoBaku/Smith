// // Fill out your copyright notice in the Description page of Project Settings.

// #include "LogController.h"
// #include "GameLogWidget.h"
// #include "TestSendLog.h"
// #include "Debug.h"

// // Sets default values
// ALogController::ALogController()
// 		: m_widgetClass(nullptr)
// 		, m_widget(nullptr)
// 		,	m_testSend(nullptr)
// 		, timer(0.0f)
// 		, cnt(0)
// {
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;

// 	// 生成したいwidgetのpath
// 	FString Path = TEXT("/Game/TestLevel/BP/BP_GameLogWidget.BP_GameLogWidget_C");
// 	m_widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

// 	if (m_widgetClass == nullptr)
// 	{
// 		MDebug::LogError("Not Found widget Class");
// 	}
// 	m_testSend = MakeShared<TestSendLog>();
// }

// // Called when the game starts or when spawned
// void ALogController::BeginPlay()
// {
// 	Super::BeginPlay();
// 	m_widget = CreateWidget<UGameLogWidget>(GetWorld(), m_widgetClass);

// 	if (m_widget == nullptr)
// 	{
// 		MDebug::LogError(TEXT("widgetがnull"));
// 		return;
// 	}

// 	m_mediator = MakeShared<GameLogMediator>();

// 	m_mediator->SetUserWidget(m_widget);

// 	m_widget->AddToViewport();
// }

// // Called every frame
// void ALogController::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	timer += DeltaTime;

// 	// test
// 	if (timer >= 3.0f)
// 	{
// 		if (IsValid(m_widget))
// 		{
// 			if(nullptr != m_testSend)
// 			{
// 			 m_mediator->OutPutDamageLog(m_testSend.Get(),10);
// 			}
// 			else
// 			{
// 				MDebug::LogError(TEXT("失敗"));
// 			}
// 		}
// 		else
// 		{
// 			MDebug::LogError("null Error");
// 		}
// 		timer = 0.0f;
// 	}
// }