// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithModelHelperFunctionLibrary.h"

#include "MapCoord.h"
#include "Direction.h"
#include "MLibrary.h"

class IDirectionStrategy
{
public:
  IDirectionStrategy() = default;
  virtual ~IDirectionStrategy() = default;

public:
  virtual EDirection GetDirectionOfDegree(double angleDegree) = 0;
};

class OrdinalDirectionStrategy final: public IDirectionStrategy
{
public:
  OrdinalDirectionStrategy() = default;
  ~OrdinalDirectionStrategy() = default;

public:
  EDirection GetDirectionOfDegree(double angleDegree) override
  {
    if (-22.5 < angleDegree && angleDegree < 22.5)
    {
      return EDirection::North;
    }

    if (22.5 <= angleDegree && angleDegree <= 67.5)
    {
      return EDirection::NorthEast;
    }

    if (67.5 < angleDegree && angleDegree < 112.5)
    {
      return EDirection::East;
    }

    if (112.5 <= angleDegree && angleDegree <= 157.5)
    {
      return EDirection::SouthEast;
    }

    if (-67.5 <= angleDegree && angleDegree <= -22.5)
    {
      return EDirection::NorthWest;
    }

    if (-112.5 < angleDegree && angleDegree < -67.5)
    {
      return EDirection::West;
    }

    if (-157.5 <= angleDegree && angleDegree <= -112.5)
    {
      return EDirection::SouthWest;
    }

    return EDirection::South;
  }
};

class CardinalDirectionStrategy final: public IDirectionStrategy
{
public:
  CardinalDirectionStrategy() = default;
  ~CardinalDirectionStrategy() = default;

public:
  EDirection GetDirectionOfDegree(double angleDegree) override
  {
    if (-45.0 <= angleDegree && angleDegree < 45.0)
    {
      return EDirection::North;
    }
  
    if (45.0 <= angleDegree && angleDegree < 135.0)
    {
      return EDirection::East;
    }
  
    if (-135.0 <= angleDegree && angleDegree < -45.0)
    {
      return EDirection::West;
    }
  
    return EDirection::South;
  }
};

class DiagonalDirectionStrategy final: public IDirectionStrategy
{
  public:
  DiagonalDirectionStrategy() = default;
  ~DiagonalDirectionStrategy() = default;

public:
  EDirection GetDirectionOfDegree(double angleDegree) override
  {
    if (0.0 <= angleDegree && angleDegree < 90.0)
    {
      return EDirection::NorthEast;
    }
  
    if (90.0 <= angleDegree && angleDegree < 180.0)
    {
      return EDirection::SouthEast;
    }
  
    if (-90.0 <= angleDegree && angleDegree < 0.0)
    {
      return EDirection::NorthWest;
    }
  
    return EDirection::SouthWest;
  }
};


EDirection FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(const FMapCoord& from, const FMapCoord& to, EDirectionStrategy directionStrategyLevel)
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

  // TODO
  return GetDirectionOfDegree(angleTwoPI, directionStrategyLevel);
}

EDirection FSmithModelHelperFunctionLibrary::GetDirectionOfDegree(double angleDegree, EDirectionStrategy directionStrategyLevel)
{
  adjustInputAngle(angleDegree);

  TSharedPtr<IDirectionStrategy> directionStrategy = nullptr;

  switch (directionStrategyLevel)
  {
    case EDirectionStrategy::Ordinal:
    {
      directionStrategy = ::MakeShared<OrdinalDirectionStrategy>();
    }
    break;
    case EDirectionStrategy::Cardinal:
    {
      directionStrategy = ::MakeShared<CardinalDirectionStrategy>();
    }
    break;
    case EDirectionStrategy::Diagonal:
    {
      directionStrategy = ::MakeShared<DiagonalDirectionStrategy>();
    }
    break;
  }

  if (directionStrategy.IsValid())
  {
    return directionStrategy->GetDirectionOfDegree(angleDegree);
  }

  return EDirection::Invalid;

}

void FSmithModelHelperFunctionLibrary::adjustInputAngle(double& angleDegree)
{
  while (angleDegree < -180.0)
  {
    angleDegree += 180.0;
  }

  while(angleDegree > 180.0)
  {
    angleDegree -= 180.0;
  }
}
