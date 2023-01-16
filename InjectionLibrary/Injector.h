#pragma once

#include <windows.h>
#include <string>

namespace InjectLibrary
{
	class Injector
	{
	public:
		Injector(const std::string& mutexName, int windowsMessage);
		virtual ~Injector() = default;
		void CALLBACK SetHook(const HINSTANCE moduleHandle = nullptr);
	private:
		static const LRESULT CALLBACK HookMessageProcedure(const DWORD code, const DWORD wParam, const DWORD lParam);

	private:
		static HHOOK _hookHandle;
		HANDLE _mutexHandle = nullptr;
		int _windowsMessage = 0;
		const std::string _mutexName;
	};
};
