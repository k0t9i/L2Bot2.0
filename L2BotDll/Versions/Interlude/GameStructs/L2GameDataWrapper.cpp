#include "pch.h"
#include "../../../Common/apihook.h"
#include "L2GameDataWrapper.h"
#include "ProcessManipulation.h"
#include "Domain/Services/ServiceLocator.h"

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
	}

	void L2GameDataWrapper::Restore()
	{
		restore(originalInitAddress);
	}

	//todo exception(?)
	FL2ItemDataBase* L2GameDataWrapper::GetItemData(int itemId) const
	{
		if (__GetItemData && _target) {
			return (*__GetItemData)(_target, itemId);
		}
		return 0;
	}

	FL2MagicSkillData* L2GameDataWrapper::GetMSData(int skillId, int level) const
	{
		if (__GetMSData && _target) {
			return (*__GetMSData)(_target, skillId, level);
		}
		return 0;
	}

	int __fastcall L2GameDataWrapper::__Init_hook(L2GameData* This, int, int unk, int unk1)
	{
		if (_target == 0) {
			_target = This;

			InjectLibrary::StopCurrentProcess();
			restore(originalInitAddress);
			InjectLibrary::StartCurrentProcess();

			Services::ServiceLocator::GetInstance().GetLogger()->Info(L"FL2GameData {:#010x} obtained", (int)_target);
			return (*__Init)(This, unk, unk1);
		}

		return 0;
	}
};
