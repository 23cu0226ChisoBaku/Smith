// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/Params.h"

FParams operator+(const FParams& lhs, const FParams& rhs)
{
	FParams tmp;
	tmp.HP = lhs.HP + rhs.HP;
	tmp.ATK = lhs.ATK + rhs.ATK;
	tmp.DEF = lhs.DEF + rhs.DEF;
	tmp.CRT = lhs.CRT + rhs.CRT;
	return tmp;
}
