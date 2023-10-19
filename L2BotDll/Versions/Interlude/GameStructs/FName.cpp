#include "pch.h"
#include "FName.h"
#include "Domain/Exceptions.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	FNameEntry* (__cdecl* FName::__GetEntry)(int) = 0;

	FNameEntry* FName::GetEntry(int index) const
	{
		__try {
			if (__GetEntry) {
				return(*__GetEntry)(index);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"FName::GetEntry failed");
		}
	}

	void FName::Init(HMODULE hModule)
	{
		(FARPROC&)__GetEntry = GetProcAddress(hModule, "?GetEntry@FName@@SAPAUFNameEntry@@H@Z");
	}
}
