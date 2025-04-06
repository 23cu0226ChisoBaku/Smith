
#include "IMapModelGateway.h"

bool operator==(const FMapModelDTO& Lhs, const FMapModelDTO Rhs)
{
	return Lhs.ActorClass == Rhs.ActorClass;
}
