// Fill out your copyright notice in the Description page of Project Settings.


#include "Params.h"

FParams operator+(const FParams& Lhs, const FParams& Rhs)
{
	const FParams tmp =
	{
		.HP = Lhs.HP + Rhs.HP,
		.ATK = Lhs.ATK + Rhs.ATK,
		.DEF = Lhs.DEF + Rhs.DEF,
		.CRT = Lhs.CRT + Rhs.CRT
	};

	return tmp;
}

FParams& FParams::operator+=(const FParams& Rhs)
{
	HP += Rhs.HP;
	ATK += Rhs.ATK;
	DEF += Rhs.DEF;
	CRT += Rhs.CRT;

	return *this;
}

