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

		static void __fastcall __OnSkillListPacket_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static int __fastcall __OnReceiveMagicSkillUse_hook(GameEngine* This, uint32_t /*edx*/, User* u1, User* u2, L2ParamStack& stack);
		static void __fastcall __OnReceiveMagicSkillCanceled_hook(GameEngine* This, uint32_t /*edx*/, User* user);
		static void __fastcall __AddAbnormalStatus_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __AddInventoryItem_hook(GameEngine* This, uint32_t /*edx*/, ItemInfo& itemInfo);
		static void __fastcall __OnReceiveUpdateItemList_hook(GameEngine* This, uint32_t /*edx*/, UpdateItemListActionType actionType, ItemInfo& itemInfo);
		static void __fastcall __OnExAutoSoulShot_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __Tick_hook(GameEngine* This, uint32_t /*edx*/, float_t unk);
		static void __fastcall __OnSay2_hook(GameEngine* This, uint32_t /*edx*/, L2ParamStack& stack);
		static void __fastcall __OnEndItemList_hook(GameEngine* This, uint32_t /*edx*/);
		
	private:
		static GameEngine* _target;
	};
}