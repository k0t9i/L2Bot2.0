#pragma once

#include "pch.h"
#include "GameStructs.h"
#include "../../GameStructs/L2GameDataInterface.h"

namespace Interlude
{
	class L2GameDataWrapper : public L2GameDataInterface
	{
	public:
		class L2GameData {};
		L2GameDataWrapper() = default;
		virtual ~L2GameDataWrapper() = default;

		FL2ItemDataBase* GetItemData(int itemId) const;
		FL2MagicSkillData* GetMSData(int skillId, int level) const;
		void Init(HMODULE hModule) override;
		void Restore() override;
	private:
		static int(__thiscall* __Init)(L2GameData*, int, int);
		static FL2ItemDataBase* (__thiscall* __GetItemData)(L2GameData*, int);
		static FL2MagicSkillData* (__thiscall* __GetMSData)(L2GameData*, int, int);

		static int __fastcall __Init_hook(L2GameData* This, int /*edx*/, int unk, int unk1);
	private:
		static void* originalInitAddress;
		static L2GameData* _target;
	};
};
