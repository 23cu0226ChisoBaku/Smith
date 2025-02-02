// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHP.h"
#include "Blueprint/UserWidget.h"
#include "HPUIComponent.h"
#include "Debug.h"

APlayerHP::APlayerHP()
:m_percent(1.0f)
,m_maxHp(10)
{
  PrimaryActorTick.bCanEverTick = false;
	m_hpComp = CreateDefaultSubobject<UHPUIComponent>(TEXT("konno Player HP Component"));
	m_currentHp = m_maxHp;
}

// Called when the game starts or when spawned
void APlayerHP::BeginPlay()
{
	Super::BeginPlay();
	
	// // 生成したいwidgetのpath
	FString Path = TEXT("/Game/TestLevel/BP/BP_SmithPlayerHP.BP_SmithPlayerHP_C");
	TSubclassOf<UUserWidget> m_widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	if (m_widgetClass == nullptr)
	{
		MDebug::LogError("Not Found widget Class");
	}

	m_widget = CreateWidget<UUserWidget>(GetWorld(), m_widgetClass);

	if(GetWorld() == nullptr)
	{
		MDebug::LogError(TEXT("ワールドがnull!!!"));
	}

	if (m_widget == nullptr)
	{
		MDebug::LogError(TEXT("widgetがnull"));
		return;
	}

	m_widget->AddToViewport();
}

// Called every frame
void APlayerHP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerHP::OnHP()
{
	m_currentHp -= 2;

	MDebug::LogError(m_widget->GetName());
	if(m_currentHp <= 0)
	{
		MDebug::LogError(TEXT("HP減らせないよ"));
		m_percent = 0.0f;
		//m_hpComp->SetHP(0.0f);
		return;
	}

	if(m_widget == nullptr)
	{
		MDebug::LogError(TEXT("widgetがnullだよ"));
		return;
	}

	m_percent = (float)m_currentHp / (float)m_maxHp;
	//m_hpComp->SetHP(percent);
}