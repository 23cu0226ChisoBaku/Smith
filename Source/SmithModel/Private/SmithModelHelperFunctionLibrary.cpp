// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithModelHelperFunctionLibrary.h"

#include "MapCoord.h"
#include "Direction.h"
#include "MLibrary.h"

//---Begin of Direction Strategy Definition
#pragma region Direction Strategy Definition
struct FDirectionDirector
{
  using ThisClass = typename FDirectionDirector;
  
  struct IDirectionConcept
  {
    virtual ~IDirectionConcept() = 0 {};

    virtual EDirection GetDirectionOfDegree(double AngleDegree) const = 0;
  };

  template<typename StrategyType>
  struct DirectionModel : public IDirectionConcept
  {
    DirectionModel(StrategyType Strategy)
      : m_strategy(::MoveTemp(Strategy))
    { }

    ~DirectionModel() = default;

    EDirection GetDirectionOfDegree(double AngleDegree) const override 
    {
      return m_strategy.GetDirection(AngleDegree);
    }

    private:
    StrategyType m_strategy;
  };

  public:

    template<typename StrategyType>
    explicit FDirectionDirector(StrategyType Strategy)
      : m_pimpl(::MakeShared<DirectionModel<StrategyType>>(::MoveTemp(Strategy)))
    { }

    ~FDirectionDirector()
    {
      m_pimpl.Reset();
    }

    FDirectionDirector(const ThisClass& Other) = default;
    ThisClass& operator=(const ThisClass& Other) = default;
    FDirectionDirector(ThisClass&& Other) noexcept = default;
    ThisClass& operator=(ThisClass&& Other) noexcept = default;

    EDirection GetDirectionOfDegree(double AngleDegree) const
    {
      return m_pimpl->GetDirectionOfDegree(AngleDegree);
    }

  private:

    TSharedPtr<IDirectionConcept> m_pimpl;
};

struct DirectionStrategy_Ordinal
{
  EDirection GetDirection(double AngleDegree) const
  {
    if (-22.5 < AngleDegree && AngleDegree < 22.5)
    {
      return EDirection::North;
    }

    if (22.5 <= AngleDegree && AngleDegree <= 67.5)
    {
      return EDirection::NorthEast;
    }

    if (67.5 < AngleDegree && AngleDegree < 112.5)
    {
      return EDirection::East;
    }

    if (112.5 <= AngleDegree && AngleDegree <= 157.5)
    {
      return EDirection::SouthEast;
    }

    if (-67.5 <= AngleDegree && AngleDegree <= -22.5)
    {
      return EDirection::NorthWest;
    }

    if (-112.5 < AngleDegree && AngleDegree < -67.5)
    {
      return EDirection::West;
    }

    if (-157.5 <= AngleDegree && AngleDegree <= -112.5)
    {
      return EDirection::SouthWest;
    }

    return EDirection::South;
  }
};

struct DirectionStrategy_Cardinal
{
  EDirection GetDirection(double AngleDegree) const
  {
    if (-45.0 <= AngleDegree && AngleDegree < 45.0)
    {
      return EDirection::North;
    }
  
    if (45.0 <= AngleDegree && AngleDegree < 135.0)
    {
      return EDirection::East;
    }
  
    if (-135.0 <= AngleDegree && AngleDegree < -45.0)
    {
      return EDirection::West;
    }
  
    return EDirection::South;
  }
};

struct DirectionStrategy_Diagonal
{
  EDirection GetDirection(double AngleDegree) const
  {
    if (0.0 <= AngleDegree && AngleDegree < 90.0)
    {
      return EDirection::NorthEast;
    }
  
    if (90.0 <= AngleDegree && AngleDegree < 180.0)
    {
      return EDirection::SouthEast;
    }
  
    if (-90.0 <= AngleDegree && AngleDegree < 0.0)
    {
      return EDirection::NorthWest;
    }
  
    return EDirection::SouthWest;
  }
};
#pragma endregion Direction Strategy Definition
//---End of Direction Strategy Definition

EDirection FSmithModelHelperFunctionLibrary::GetDirectionOfMapCoord(const FMapCoord& From, const FMapCoord& To, EDirectionPolicy Policy)
{
  const double directionVectorX = StaticCast<double>(To.x) - StaticCast<double>(From.x);
  const double directionVectorY = StaticCast<double>(To.y) - StaticCast<double>(From.y);
  const FVector2D directionVector = FVector2D(directionVectorX, directionVectorY).GetSafeNormal();

  if (directionVector.IsZero())
  {
    return EDirection::Invalid;
  }

  double angleTwoPI = 0.0;
  const double crossWithNorthVec = FVector2D::CrossProduct(directionVector, FVector2D{1.0, 0.0});
  
  const double tolerance = 0.000001; 
  if (FMath::Abs(crossWithNorthVec) <= tolerance)
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
    angleTwoPI = (crossWithNorthVec < 0.0) ? angle : -angle;
  }

  return GetDirectionOfDegree(angleTwoPI, Policy);
}

EDirection FSmithModelHelperFunctionLibrary::GetDirectionOfDegree(double AngleDegree, EDirectionPolicy Policy)
{
  adjustInputAngle(AngleDegree);

  using enum EDirectionPolicy;
  static const TMap<EDirectionPolicy, const FDirectionDirector> DIRECTION_DIRECTOR_TABLE =
  {
    {Ordinal, FDirectionDirector(DirectionStrategy_Ordinal{})},
    {Cardinal, FDirectionDirector(DirectionStrategy_Cardinal{})},
    {Diagonal, FDirectionDirector(DirectionStrategy_Diagonal{})}
  };

  return DIRECTION_DIRECTOR_TABLE[Policy].GetDirectionOfDegree(AngleDegree);
}

void FSmithModelHelperFunctionLibrary::adjustInputAngle(double& AngleDegree)
{
  while (AngleDegree < -180.0)
  {
    AngleDegree += 180.0;
  }

  while(AngleDegree > 180.0)
  {
    AngleDegree -= 180.0;
  }
}
