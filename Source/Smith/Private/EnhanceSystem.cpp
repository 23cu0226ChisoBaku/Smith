#include "EnhanceSystem.h"
#include "ParamAbsorbable.h"
#include "IEnhanceable.h"
#include "Params.h"

#include "Debug.h"

EnhanceSystem::EnhanceSystem()
{
}

void EnhanceSystem::Enhance(IEnhanceable* enhanceable,IParamAbsorbable* absorbItem)
{
  // if 
  // FParams enhancePoint = weapon->GetParam();
  // enhancePoint.HP += item.HP;
  // enhancePoint.ATK += item.ATK;
  // enhancePoint.DEF += item.DEF;
  // enhancePoint.CRT += item.CRT;

  // weapon->SetParam(enhancePoint);
}