#pragma once

#include "UObject/ObjectMacros.h"

// Namespaced to avoid unecessarily (in this case) leaving values of this enum to global scope.

namespace EGameState
{
	enum Type : uint8
	{
		EGS_Failed = 0,
		EGS_Scored
	};
};