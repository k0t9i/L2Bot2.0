#pragma once

#include <Windows.h>

class NetworkHandlerInterface
{
public:
	NetworkHandlerInterface() = default;
	virtual ~NetworkHandlerInterface() = default;

	virtual void Init(HMODULE hModule) = 0;
	virtual void Restore() = 0;
};