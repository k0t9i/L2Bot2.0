#include "pch.h"
#include "Injector.h"

namespace InjectLibrary
{
	HHOOK Injector::_hookHandle = nullptr;

	Injector::Injector(const std::string& mutexName, int windowsMessage) : _mutexName(mutexName), _windowsMessage(windowsMessage)
	{
	}

	void Injector::SetHook(const HINSTANCE moduleHandle)
	{
		if (moduleHandle) {
			// С помощью мютекса проверяем, что еще не было хука 
			HANDLE mutexHandle = CreateMutexA(nullptr, false, _mutexName.c_str());
			if (GetLastError() != ERROR_ALREADY_EXISTS) {
				_hookHandle = SetWindowsHookExA(_windowsMessage, (HOOKPROC)HookMessageProcedure, moduleHandle, 0);
				_mutexHandle = mutexHandle;
			}
			else if (mutexHandle) {
				CloseHandle(mutexHandle);
			}
		}
		else {
			UnhookWindowsHookEx(_hookHandle);
			CloseHandle(_mutexHandle);
		}
	}

	const LRESULT Injector::HookMessageProcedure(const DWORD code, const DWORD wParam, const DWORD lParam)
	{
		return CallNextHookEx(_hookHandle, code, wParam, lParam);
	}
}