#include "pch.h"
#include "Common/apihook.h"
#include "Application.h"
#include "ProcessManipulation.h"
#include "Injector.h"

InjectLibrary::Injector injector("L2BotHookMutex", WH_GETMESSAGE);
Application application(VersionAbstractFactory::Version::interlude);

void ConfigLogger(HMODULE hModule)
{
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(hModule, buf, MAX_PATH);
	const std::wstring libName(buf);

	std::wstring directory;
	const size_t lastSlashIndex = libName.rfind(L"\\");
	if (std::string::npos != lastSlashIndex)
	{
		directory = libName.substr(0, lastSlashIndex);
	}

	std::vector <std::unique_ptr<Logger::LogChannel>> channels;
#ifdef _DEBUG
	channels.push_back(std::make_unique<OutputDebugLogChannel>(std::vector<Logger::LogLevel>{}));
#endif
	channels.push_back(std::make_unique<FileLogChannel>(directory + L"\\app.log", std::vector<Logger::LogLevel>{
#ifndef _DEBUG
		Logger::LogLevel::error,
		Logger::LogLevel::warning
#endif
	}));
	channels.push_back(std::make_unique<ChatLogChannel>(Enums::ChatChannelEnum::log, std::vector<Logger::LogLevel>{
		Logger::LogLevel::app
	}));
	Services::ServiceLocator::GetInstance().SetLogger(std::make_unique<Logger::Logger>(std::move(channels)));
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	const int processId = GetCurrentProcessId();
	const std::string& processName = InjectLibrary::GetCurrentProcessName();

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		injector.SetHook(hModule);
		if (processName == "l2.exe") {
			ConfigLogger(hModule);

			InjectLibrary::StopCurrentProcess();
			application.Start();
			InjectLibrary::StartCurrentProcess();
			Services::ServiceLocator::GetInstance().GetLogger()->Info(L"attached to Lineage 2 client {:#010x}", processId);
		}
		break;
	case DLL_PROCESS_DETACH:
		if (processName == "l2.exe") {
			InjectLibrary::StopCurrentProcess();
			application.Stop();
			InjectLibrary::StartCurrentProcess();
			Services::ServiceLocator::GetInstance().GetLogger()->Info(L"detached from Lineage 2 client {:#010x}", processId);
		}
		injector.SetHook();
		break;
	}
	return TRUE;
}