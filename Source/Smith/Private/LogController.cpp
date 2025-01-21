// Fill out your copyright notice in the Description page of Project Settings.

#include "LogController.h"
#include "GameLogWidget.h"
#include "Debug.h"

// Sets default values
ALogController::ALogController()
		: m_widgetClass(nullptr)
		, m_widget(nullptr)
		, timer(0.0f)
		, cnt(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString Path = TEXT("/Game/TestLevel/BP/BP_GameLogWidget.BP_GameLogWidget_C");
	m_widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	if (m_widgetClass == nullptr)
	{
		MDebug::LogError("Not Found widget Class");
	}
}

// Called when the game starts or when spawned
void ALogController::BeginPlay()
{
	Super::BeginPlay();
	m_widget = CreateWidget<UUserWidget>(GetWorld(), m_widgetClass);

	m_mediator = MakeShared<GameLogMediator>();

	m_mediator->SetUserWidget(m_widget);

	if(m_widget == nullptr)
	{
		MDebug::LogError(TEXT("なんでエラーなの"));
	}

	if (m_widget != nullptr)
	{
		m_widget->AddToViewport();
	}
	else
	{
		MDebug::LogError(TEXT("nullです"));
	}
}

// Called every frame
void ALogController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	if (timer >= 3.0f)
	{
		if (IsValid(m_widget))
		{
			UGameLogWidget *widget = Cast<UGameLogWidget>(m_widget);
			if (nullptr != widget)
			{
				
			}
			else
			{
				MDebug::LogError("Not Cast");
			}
		}
		else
		{
			MDebug::LogError("null Error");
		}
		timer = 0.0f;
	}
}