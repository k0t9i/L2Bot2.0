#include "pch.h"
#include "../../../Common/apihook.h"
#include "L2GameDataWrapper.h"
#include "ProcessManipulation.h"
#include "Domain/Services/ServiceLocator.h"
#include "Domain/Exceptions.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	void* L2GameDataWrapper::originalInitAddress = 0;
	L2GameDataWrapper::L2GameData* L2GameDataWrapper::_target = 0;

	int(__thiscall* L2GameDataWrapper::__Init)(L2GameData*, int, int) = 0;
	FL2ItemDataBase* (__thiscall* L2GameDataWrapper::__GetItemData)(L2GameData*, int) = 0;
	FL2MagicSkillData* (__thiscall* L2GameDataWrapper::__GetMSData)(L2GameData*, int, int) = 0;

	void L2GameDataWrapper::Init(HMODULE hModule)
	{
		void* initAddress = GetProcAddress(hModule, "?GetMSData@FL2GameData@@QAEPAUFL2MagicSkillData@@HH@Z");
		originalInitAddress = splice(initAddress, __Init_hook);
		(FARPROC&)__Init = (FARPROC)initAddress;

		(FARPROC&)__GetItemData = GetProcAddress(hModule, "?GetItemData@FL2GameData@@QAEPAVFL2ItemDataBase@@H@Z");
		(FARPROC&)__GetMSData = GetProcAddress(hModule, "?GetMSData@FL2GameData@@QAEPAUFL2MagicSkillData@@HH@Z");
		Services::ServiceLocator::GetInstance().GetLogger()->Info(L"FL2GameData hooks initialized");
	}

	void L2GameDataWrapper::Restore()
	{
		restore(originalInitAddress);
		Services::ServiceLocator::GetInstance().GetLogger()->Info(L"FL2GameData hooks restored");
	}

	FL2ItemDataBase* L2GameDataWrapper::GetItemData(int itemId) const
	{
		__try {
			if (__GetItemData && _target) {
				return (*__GetItemData)(_target, itemId);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"FL2GameData::GetItemData failed");
		}
	}

	FL2MagicSkillData* L2GameDataWrapper::GetMSData(int skillId, int level) const
	{
		__try {
			if (__GetMSData && _target) {
				return (*__GetMSData)(_target, skillId, level);
			}
			return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"FL2GameData::GetMSData failed");
		}
	}

	int __fastcall L2GameDataWrapper::__Init_hook(L2GameData* This, int, int unk, int unk1)
	{
		if (_target == 0) {
			_target = This;
			Services::ServiceLocator::GetInstance().GetLogger()->Info(L"FL2GameData pointer {:#010x} obtained", (int)_target);

			InjectLibrary::StopCurrentProcess();
			restore(originalInitAddress);
			InjectLibrary::StartCurrentProcess();

			return (*__Init)(This, unk, unk1);
		}

		return 0;
	}
};
