// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <cassert>
#include <type_traits>

/**
 * 
 */
class MLIBRARY_API GetAddComponent
{
public:
	template<typename UnrealComponent>
	UnrealComponent* GetComponent(AActor*);

	template<typename UnrealComponent>
	UnrealComponent* AddComponent(AActor*);

	template<typename UnrealComponent>
	UnrealComponent* GetOrAddComponent(AActor*);

};

template<typename UnrealComponent>
UnrealComponent* GetComponent(AActor* actor)
{
	static_assert(sizeof(UnrealComponent) > 0, "Can't use incomplete Type");
	static_assert(std::is_convertible<UnrealComponent, UActorComponent>::value, "Can't convert to UActorComponent");

	if (::IsValid(actor))
	{
		
	}
}

#define GetComponent(ActorPtr, ComponentType) 			GetAddComponent::GetComponent<decltype(ComponentType)>(ActorPtr);
#define AddComponent(ActorPtr, ComponentType) 			GetAddComponent::AddComponent<decltype(ComponentType)>(ActorPtr);
#define GetOrAddComponent(ActorPtr, ComponentType) 	GetAddComponent::GetOrAddComponent<decltype(ComponentType)>(ActorPtr);
