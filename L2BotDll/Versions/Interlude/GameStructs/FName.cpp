#include "pch.h"
#include "FName.h"

namespace Interlude
{
	FNameEntry* (__cdecl* FName::__GetEntry)(int) = 0;

	//todo exception(?)
	FNameEntry* FName::GetEntry(int index) const
	{
		if (__GetEntry) {
			return(*__GetEntry)(index);
		}
		return 0;
	}

	void FName::Init(HMODULE hModule)
	{
		(FARPROC&)__GetEntry = GetProcAddress(hModule, "?GetEntry@FName@@SAPAUFNameEntry@@H@Z");
	}
}
