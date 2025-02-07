// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerHP.h"
#include "MLibrary.h"

void USmithPlayerHP::SetHP(float _percent)
{
  percent = _percent;
  UpdateHP();
}