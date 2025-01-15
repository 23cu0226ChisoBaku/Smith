// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/TestEnhanceActor.h"
#include "TestWeapon.h"
#include "EnhanceSystem.h"

// Sets default values
ATestEnhanceActor::ATestEnhanceActor()
:m_enhanceSystem(nullptr)
,m_weapon(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestEnhanceActor::BeginPlay()
{
	Super::BeginPlay();
	m_weapon = MakeShared<TestWeapon>();
	m_enhanceSystem = MakeShared<EnhanceSystem>();

	FParams params = {30,10,5,15};
	EnhanceAction(params);
}

// Called every frame
void ATestEnhanceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestEnhanceActor::EnhanceAction(FParams params)
{
	if(m_enhanceSystem != nullptr || m_weapon != nullptr)
	{
		m_enhanceSystem->Enhance(m_weapon.Get(), params);
	}
}
