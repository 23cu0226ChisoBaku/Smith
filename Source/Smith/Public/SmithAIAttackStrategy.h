// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "SmithAIAttackStrategy.generated.h"

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
	}
}

UCLASS(Abstract)
class SMITH_API USmithAIAttackStrategy : public USmithAIStrategy
{
	GENERATED_BODY()

public:
	USmithAIAttackStrategy(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	bool RegisterAttackFormat(const FString& name, const UDataTable* formatTable);

private:
	virtual bool executeImpl() PURE_VIRTUAL(USmithAIAttackStrategy::executeImpl, return false;);

protected:
	TMap<FString, TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>> m_attackFormatTables;

};