// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAP_MODEL
#define SMITH_MAP_MODEL

class USmithMapModelDefinition;
enum class EMapModelType : uint8;

/// @brief ダンジョンモデル
struct SMITHMODEL_API FSmithMapModel
{
	friend class USmithMapModelRepository;

	private:
	
		FSmithMapModel() = default;

		static FSmithMapModel CreateModel(USmithMapModelDefinition* Definition);

	public:

		uint8 GetSizeX() const;

		uint8 GetSizeY() const;

		EMapModelType GetModelType() const;

		bool IsValid() const;

	private:

		TWeakObjectPtr<USmithMapModelDefinition> m_definition;
};

#endif