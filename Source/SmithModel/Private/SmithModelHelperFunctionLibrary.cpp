// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithModelHelperFunctionLibrary.h"
#include "MapCoord.h"
#include "Direction.h"
#include "MLibrary.h"


EDirection FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(const FMapCoord& from, const FMapCoord& to)
{
  const FVector2D directionVector = FVector2D(
                                              StaticCast<double>(to.x) - StaticCast<double>(from.x),
                                              StaticCast<double>(to.y) - StaticCast<double>(from.y)
                                             ).GetSafeNormal();

  if (directionVector.IsZero())
  {
    return EDirection::Invalid;
  }

  double angleTwoPI = 0.0;
  const double crossWithNorthVec = FVector2D::CrossProduct(directionVector, FVector2D{1.0, 0.0});
  
  if (crossWithNorthVec == 0.0)
  {
    if (directionVector.X > 0.0)
    {
      angleTwoPI = 0.0;
    }
    else
    {
      angleTwoPI = 180.0;
    }
  }
  else
  {
    const double angleBetweenNorthVec = FVector2D::DotProduct(directionVector, FVector2D{1.0, 0.0});
    const double angle = FMath::RadiansToDegrees(FMath::Acos(angleBetweenNorthVec));
    angleTwoPI = crossWithNorthVec < 0.0 ? angle : -angle;
  }

  if (-30.0 < angleTwoPI && angleTwoPI < 30.0)
  {
    return EDirection::North;
  }

  if (30.0 <= angleTwoPI && angleTwoPI <= 60.0)
  {
    return EDirection::NorthEast;
  }

  if (60 < angleTwoPI && angleTwoPI < 120.0)
  {
    return EDirection::East;
  }

  if (120.0 <= angleTwoPI && angleTwoPI <= 150.0)
  {
    return EDirection::SouthEast;
  }

  if (-60.0 <= angleTwoPI && angleTwoPI <= -30.0)
  {
    return EDirection::NorthWest;
  }

  if (-120.0 < angleTwoPI && angleTwoPI < -60.0)
  {
    return EDirection::West;
  }

  if (-150.0 <= angleTwoPI && angleTwoPI <= -120.0)
  {
    return EDirection::SouthWest;
  }

  return EDirection::South;

}
