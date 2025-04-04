// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
enum class EMapModelType : uint8
{
  Player UMETA(DisplayName = "Player"),
  Enemy UMETA(DisplayName = "Enemy"),
  TypeCount UMETA(Hidden),
};	
