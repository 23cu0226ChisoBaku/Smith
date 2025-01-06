// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithStaticObj.h"
#include "FormatTransformer.h"
#include "FormatInfo_Import.h"
#include "AttackHandle.h"
using namespace UE::Smith::Battle;
using namespace UE::MLibrary::MDataStructure;

// Sets default values
ASmithStaticObj::ASmithStaticObj()
	: m_mapCoord({})
	, m_format()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmithStaticObj::BeginPlay()
{
	Super::BeginPlay();
	ESmithFormatType type[9];
	type[0] = ESmithFormatType::EFFECT;
	type[1] = ESmithFormatType::EFFECT;
	type[2] = ESmithFormatType::EFFECT;
	type[3] = ESmithFormatType::EFFECT;
	type[4] = ESmithFormatType::EFFECT;
	type[5] = ESmithFormatType::EFFECT;
	type[6] = ESmithFormatType::NO_EFFECT;
	type[7] = ESmithFormatType::CENTER_NO_EFFECT;
	type[8] = ESmithFormatType::NO_EFFECT;
	
	m_format.SetupFormat(type, 9 * sizeof(ESmithFormatType), 3, 3);
	const auto& test = m_format.GetFormatArray();
	if (GEngine)
	{
		for (ESmithFormatType ENUM : test)
		{
			FString x = FString::FromInt(StaticCast<uint8>(ENUM));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, x);
		}
	}

	FMapCoord cnCoordTemp{ 0, 9 };

  auto mapCoordArr = FFormatTransformer::FormatToMapCoord(m_format, cnCoordTemp);

	for (uint64 y = 0 ; y < mapCoordArr.Row(); ++y)
	{
		FString rowString = {};
		for (uint64 x = 0 ; x < mapCoordArr.Column(); ++x)
		{
			rowString += "{ ";
			rowString += FString::FromInt(mapCoordArr.At_ReadOnly(y, x).x);
			rowString += " , ";
			rowString += FString::FromInt(mapCoordArr.At_ReadOnly(y, x).y);
			rowString += " } ";
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, rowString);
		}
	}

	if (!m_dataTable.IsValid())
	{
		m_dataTable.LoadSynchronous();
	}

	m_dataRef = m_dataTable.Get();

	if (!m_dataTableDiagonal.IsValid())
	{
		m_dataTableDiagonal.LoadSynchronous();
	}

	m_dataDiagonalRef = m_dataTableDiagonal.Get();

	if (::IsValid(m_dataRef))
	{
		TArray<FFormatInfo_Import*> arr;
		arr.Reserve(10);
		m_dataRef->GetAllRows<FFormatInfo_Import>("", arr);
		for(auto b : arr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(StaticCast<uint8>(b->Type)));
			}
		}
	}

}

// Called every frame
void ASmithStaticObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnableInput(nullptr);
}

void ASmithStaticObj::OnAttack(AttackHandle&& attack)
{
	if (attack.AttackName.IsEmpty())
	{
		return;
	}
	else
	{
		if (GEngine != nullptr)
		{
			FString msg = GetName();
			msg.Append(TEXT(" attacked by "));
			msg.Append(attack.AttackName);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, msg);
		}

	}
}

