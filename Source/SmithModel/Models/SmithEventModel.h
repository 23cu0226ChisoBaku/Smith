// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_EVENT_MODEL
#define SMITH_EVENT_MODEL

class USmithEventModelDefinition;

/// @brief イベントモデル
struct SMITHMODEL_API FSmithEventModel
{
	friend class USmithEventModelRepository;
	
	private:

		FSmithEventModel() = default;

		static FSmithEventModel CreateModel(USmithEventModelDefinition* DefinitionAsset);

	public:

		FString GetSucceededMsg() const;

		FString GetFailedMsg() const;

		bool IsValid() const;

	private:

		TWeakObjectPtr<USmithEventModelDefinition> m_definition;
};

#endif
