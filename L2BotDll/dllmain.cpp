#include "pch.h"
#include "Common/apihook.h"
#include "Application.h"
#include "ProcessManipulation.h"
#include "Injector.h"

InjectLibrary::Injector injector("L2BotHookMutex", WH_CALLWNDPROC);
Application application(VersionAbstractFactory::Version::interlude);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	const std::string& processName = InjectLibrary::GetCurrentProcessName();
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		injector.SetHook(hModule);
		if (processName == "l2.exe") {
			InjectLibrary::StopCurrentProcess();
			application.Start();
			InjectLibrary::StartCurrentProcess();
		}
		break;
	case DLL_PROCESS_DETACH:
		if (processName == "l2.exe") {
			InjectLibrary::StopCurrentProcess();
			application.Stop();
			InjectLibrary::StartCurrentProcess();
		}
		injector.SetHook();
		break;
	}
	return TRUE;
}