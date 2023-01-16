#pragma once

#include "GameStructs.h"

class L2GameDataInterface
{
public:
	L2GameDataInterface() = default;
	virtual ~L2GameDataInterface() = default;

	virtual void Init(HMODULE hModule) = 0;
	virtual void Restore() = 0;
};