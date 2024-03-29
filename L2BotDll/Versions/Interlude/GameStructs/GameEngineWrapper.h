#pragma once

#include <math.h>
#include <cstdint>
#include "../../GameStructs/GameEngineInterface.h"
#include "GameStructs.h"
#include "L2ParamStack.h"

namespace Interlude
{
	class GameEngineWrapper : public GameEngineInterface
	{
	public:
		class GameEngine {};
		GameEngineWrapper() = default;
		virtual ~GameEngineWrapper() = default;

		void Init(HMODULE hModule) override;
		void Restore() override;

	private:
		static void(__thiscall* __OnSkillListPacket)(GameEngine*, L2ParamStack& stack);
		static int(__thiscall* __OnReceiveMagicSkillUse)(GameEngine*, User*, User*, L2ParamStack&);
		static void(__thiscall* __OnReceiveMagicSkillCanceled)(GameEngine*, User*);
		static void(__thiscall* __AddAbnormalStatus)(GameEngine*, L2ParamStack&);
		static void(__thiscall* __AddInventoryItem)(GameEngine*, ItemInfo&);
		static void(__thiscall* __OnReceiveUpdateItemList)(GameEngine*, UpdateItemListActionType, ItemInfo&);
		static void(__thiscall* __OnExAutoSoulShot)(GameEngine*, L2ParamStack&);
		static void(__thiscall* __Tick)(GameEngine*, float_t);
		static void(__thiscall* __OnSay2)(GameEngine*, L2ParamStack&);
		static void(__thiscall* __OnEndItemList)(GameEngine*);
		static float(__thiscall* __GetMaxTickRate)(GameEngine*);
		static int(__thiscall* __OnDie)(GameEngine*, User*, L2ParamStack&);
		static int(__thiscall* __OnAttack)(GameEngine*, User*, User*, int, int, int, int, int, L2::FVector, int);

		static void __fastcall __OnSkillListPacket_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static int __fastcall __OnReceiveMagicSkillUse_hook(GameEngine* This, uint32_t /*edx*/, User* attacker, User* target, L2ParamStack& stack);
		static void __fastcall __OnReceiveMagicSkillCanceled_hook(GameEngine* This, uint32_t /*edx*/, User* user);
		static void __fastcall __AddAbnormalStatus_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __AddInventoryItem_hook(GameEngine* This, uint32_t /*edx*/, ItemInfo& itemInfo);
		static void __fastcall __OnReceiveUpdateItemList_hook(GameEngine* This, uint32_t /*edx*/, UpdateItemListActionType actionType, ItemInfo& itemInfo);
		static void __fastcall __OnExAutoSoulShot_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __Tick_hook(GameEngine* This, uint32_t /*edx*/, float_t unk);
		static void __fastcall __OnSay2_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __OnEndItemList_hook(GameEngine* This, uint32_t /*edx*/);
		static int __fastcall __OnDie_hook(GameEngine* This, int /*edx*/, User* creature, L2ParamStack& stack);
		static float __fastcall __GetMaxTickRate_hook(GameEngine* This, int /*edx*/);
		static int __fastcall __OnAttack_hook(GameEngine* This, int /*edx*/, User* attacker, User* target, int unk0, int unk1, int unk2, int unk3, int unk4, L2::FVector unk5, int unk6);

	private:
		static GameEngine* _target;
	};
}