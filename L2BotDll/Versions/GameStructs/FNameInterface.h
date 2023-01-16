#pragma once

#include "GameStructs.h"

class FNameInterface
{
public:
	FNameInterface() = default;
	virtual ~FNameInterface() = default;

	virtual void Init(HMODULE hModule) = 0;
};