// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/Private/Weapon.h"


// Sets default values
AWeapon::AWeapon()
	:m_params((FParams(10, 10, 10, 10)))
	,m_currentParams(m_params)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FParams AWeapon::GetParam()
{
	return m_currentParams;
}

void AWeapon::AddParam(FParams Params)
{
	m_currentParams = m_currentParams + Params;
}

void AWeapon::SwapSkill(int, Skill)
{
	MDebug::LogError("called SwapSkill");
}

void AWeapon::Upgrade(IParamAbsorbable* material)
{
	if (material == nullptr)
	{
		return;
	}
	AddParam(material->GetParam());
	MDebug::LogError("called Upgrade");
}

