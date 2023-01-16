#pragma once

#include <windows.h>
#include <string>

namespace InjectLibrary
{
	void StartProcess(const DWORD processId);
	void StartCurrentProcess();
	void StopProcess(const DWORD processId);
	void StopCurrentProcess();
	const std::string GetProcessName(const DWORD processId);
	const std::string GetCurrentProcessName();
};
