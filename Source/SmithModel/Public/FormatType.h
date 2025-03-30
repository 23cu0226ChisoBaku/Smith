// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
enum class ESmithFormatType : uint8
{
	NO_EFFECT = 0,
	CENTER_NO_EFFECT = 1,
	EFFECT = 2,
	CENTER_EFFECT = CENTER_NO_EFFECT | EFFECT,

};
