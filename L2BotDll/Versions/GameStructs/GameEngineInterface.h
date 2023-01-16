#pragma once

#include <Windows.h>

class GameEngineInterface
{
public:
	GameEngineInterface() = default;
	virtual ~GameEngineInterface() = default;

	virtual void Init(HMODULE hModule) = 0;
	virtual void Restore() = 0;
};