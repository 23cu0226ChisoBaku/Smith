// Fill out your copyright notice in the Description page of Project Settings.


#include "Params.h"
FParams& FParams::operator+=(const FParams& other)
{
	HP += other.HP;
	ATK += other.ATK;
	DEF += other.DEF;
	CRT += other.CRT;

	return *this;
}

FParams operator+(const FParams& lhs, const FParams& rhs)
{
	FParams tmp;
	tmp.HP = lhs.HP + rhs.HP;
	tmp.ATK = lhs.ATK + rhs.ATK;
	tmp.DEF = lhs.DEF + rhs.DEF;
	tmp.CRT = lhs.CRT + rhs.CRT;
	return tmp;
}
