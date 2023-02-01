#include "pch.h"
#include "../../../Common/apihook.h"
#include "../../../Common/Common.h"
#include "GameEngineWrapper.h"
#include "ProcessManipulation.h"
#include "../../../Events/SkillCreatedEvent.h"
#include "../../../Events/SkillUsedEvent.h"
#include "../../../Events/SkillCancelledEvent.h"
#include "../../../Events/AbnormalEffectChangedEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/ItemCreatedEvent.h"
#include "../../../Events/ItemUpdatedEvent.h"
#include "../../../Events/ItemDeletedEvent.h"
#include "../../../Events/ItemAutousedEvent.h"
#include "../../../Events/GameEngineTickedEvent.h"
#include "../../../Events/ChatMessageCreatedEvent.h"
#include "../../../Events/OnEndItemListEvent.h"
#include "../../../Events/CreatureDiedEvent.h"
#include "../../../DTO/ItemData.h"
#include "FName.h"

namespace Interlude
{
	GameEngineWrapper::GameEngine* GameEngineWrapper::_target = 0;

	void(__thiscall* GameEngineWrapper::__OnSkillListPacket)(GameEngine*, L2ParamStack&) = 0;
	int(__thiscall* GameEngineWrapper::__OnReceiveMagicSkillUse)(GameEngine*, User*, User*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__OnReceiveMagicSkillCanceled)(GameEngine*, User*) = 0;
	void(__thiscall* GameEngineWrapper::__AddAbnormalStatus)(GameEngine*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__AddInventoryItem)(GameEngine*, ItemInfo&) = 0;
	void(__thiscall* GameEngineWrapper::__OnReceiveUpdateItemList)(GameEngine*, UpdateItemListActionType, ItemInfo&) = 0;
	void(__thiscall* GameEngineWrapper::__OnExAutoSoulShot)(GameEngine*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__Tick)(GameEngine*, float_t) = 0;
	void(__thiscall* GameEngineWrapper::__OnSay2)(GameEngine*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__OnEndItemList)(GameEngine*) = 0;
	float(__thiscall* GameEngineWrapper::__GetMaxTickRate)(GameEngine*) = 0;
	int(__thiscall* GameEngineWrapper::__OnDie)(GameEngine*, User*, L2ParamStack&) = 0;


	void GameEngineWrapper::Init(HMODULE hModule)
	{
		(FARPROC&)__Tick = (FARPROC)splice(
			GetProcAddress(hModule, "?Tick@UGameEngine@@UAEXM@Z"), __Tick_hook
		);
		(FARPROC&)__OnSkillListPacket = (FARPROC)splice(
			GetProcAddress(hModule, "?OnSkillListPacket@UGameEngine@@UAEXAAVL2ParamStack@@@Z"), __OnSkillListPacket_hook
		);
		(FARPROC&)__OnReceiveMagicSkillUse = (FARPROC)splice(
			GetProcAddress(hModule, "?OnReceiveMagicSkillUse@UGameEngine@@UAEXPAUUser@@0AAVL2ParamStack@@@Z"), __OnReceiveMagicSkillUse_hook
		);
		(FARPROC&)__OnReceiveMagicSkillCanceled = (FARPROC)splice(
			GetProcAddress(hModule, "?OnReceiveMagicSkillCanceled@UGameEngine@@UAEXPAUUser@@@Z"), __OnReceiveMagicSkillCanceled_hook
		);
		(FARPROC&)__AddAbnormalStatus = (FARPROC)splice(
			GetProcAddress(hModule, "?AddAbnormalStatus@UGameEngine@@UAEXAAVL2ParamStack@@@Z"), __AddAbnormalStatus_hook
		);
		(FARPROC&)__AddInventoryItem = (FARPROC)splice(
			GetProcAddress(hModule, "?AddInventoryItem@UGameEngine@@UAEXAAUItemInfo@@@Z"), __AddInventoryItem_hook
		);
		(FARPROC&)__OnReceiveUpdateItemList = (FARPROC)splice(
			GetProcAddress(hModule, "?OnReceiveUpdateItemList@UGameEngine@@UAEXHAAUItemInfo@@@Z"), __OnReceiveUpdateItemList_hook
		);
		(FARPROC&)__OnExAutoSoulShot = (FARPROC)splice(
			GetProcAddress(hModule, "?OnExAutoSoulShot@UGameEngine@@UAEXAAVL2ParamStack@@@Z"), __OnExAutoSoulShot_hook
		);
		(FARPROC&)__OnSay2 = (FARPROC)splice(
			GetProcAddress(hModule, "?OnSay2@UGameEngine@@UAEXAAVL2ParamStack@@@Z"), __OnSay2_hook
		);
		(FARPROC&)__OnEndItemList = (FARPROC)splice(
			GetProcAddress(hModule, "?OnEndItemList@UGameEngine@@UAEXXZ"), __OnEndItemList_hook
		);
		(FARPROC&)__GetMaxTickRate = (FARPROC)splice(
			GetProcAddress(hModule, "?GetMaxTickRate@UGameEngine@@UAEMXZ"), __GetMaxTickRate_hook
		);
		(FARPROC&)__OnDie = (FARPROC)splice(
			GetProcAddress(hModule, "?OnDie@UGameEngine@@UAEHPAUUser@@AAVL2ParamStack@@@Z"), __OnDie_hook
		);
	}

	void GameEngineWrapper::Restore()
	{
		restore((void*&)__OnSkillListPacket);
		restore((void*&)__OnReceiveMagicSkillUse);
		restore((void*&)__OnReceiveMagicSkillCanceled);
		restore((void*&)__AddAbnormalStatus);
		restore((void*&)__AddInventoryItem);
		restore((void*&)__OnReceiveUpdateItemList);
		restore((void*&)__OnExAutoSoulShot);
		restore((void*&)__OnSay2);
		restore((void*&)__OnEndItemList);
		restore((void*&)__GetMaxTickRate);
		restore((void*&)__OnDie);
	}

	void __fastcall GameEngineWrapper::__OnSkillListPacket_hook(GameEngine* This, uint32_t, L2ParamStack& stack)
	{
		EventDispatcher::GetInstance().Dispatch(SkillCreatedEvent{stack.GetBufferAsVector<int32_t>()});
		(*__OnSkillListPacket)(This, stack);
	}

	int __fastcall GameEngineWrapper::__OnReceiveMagicSkillUse_hook(GameEngine* This, uint32_t, User* u1, User* u2, L2ParamStack& stack)
	{
		EventDispatcher::GetInstance().Dispatch(SkillUsedEvent{ stack.GetBufferAsVector<int32_t>() });
		return (*__OnReceiveMagicSkillUse)(This, u1, u2, stack);
	}

	void __fastcall GameEngineWrapper::__OnReceiveMagicSkillCanceled_hook(GameEngine* This, uint32_t, User* user)
	{
		EventDispatcher::GetInstance().Dispatch(SkillCancelledEvent{ user->objectId });
		(*__OnReceiveMagicSkillCanceled)(This, user);
	}

	void __fastcall GameEngineWrapper::__AddAbnormalStatus_hook(GameEngine* This, uint32_t, L2ParamStack& stack)
	{
		EventDispatcher::GetInstance().Dispatch(AbnormalEffectChangedEvent{ stack.GetBufferAsVector<int32_t>(3) });
		(*__AddAbnormalStatus)(This, stack);
	}

	void __fastcall GameEngineWrapper::__AddInventoryItem_hook(GameEngine* This, uint32_t, ItemInfo& itemInfo)
	{
		EventDispatcher::GetInstance().Dispatch(
			ItemCreatedEvent
			{
				ItemData
				{
					itemInfo.objectId,
					itemInfo.itemId,
					itemInfo.amount,
					itemInfo.isEquipped,
					itemInfo.enchantLevel,
					itemInfo.mana,
					itemInfo.type2 == L2::ItemType2::QUEST,
					itemInfo.itemSlot == L2::ItemSlot::LR_HAND
				}
			}
		);
		(*__AddInventoryItem)(This, itemInfo);
	}

	void __fastcall GameEngineWrapper::__OnReceiveUpdateItemList_hook(GameEngine* This, uint32_t, UpdateItemListActionType actionType, ItemInfo& itemInfo)
	{
		const ItemData itemData
		{
			itemInfo.objectId,
			itemInfo.itemId,
			itemInfo.amount,
			itemInfo.isEquipped,
			itemInfo.enchantLevel,
			itemInfo.mana,
			itemInfo.type2 == L2::ItemType2::QUEST,
			itemInfo.itemSlot == L2::ItemSlot::LR_HAND
		};

		switch (actionType)
		{
		case UpdateItemListActionType::created:
			EventDispatcher::GetInstance().Dispatch(ItemCreatedEvent{ itemData });
			break;
		case UpdateItemListActionType::updated:
			EventDispatcher::GetInstance().Dispatch(ItemUpdatedEvent{ itemData });
			break;
		case UpdateItemListActionType::deleted:
			EventDispatcher::GetInstance().Dispatch(ItemDeletedEvent{ itemInfo.objectId });
			break;
		}
		(*__OnReceiveUpdateItemList)(This, actionType, itemInfo);
	}

	void __fastcall GameEngineWrapper::__OnExAutoSoulShot_hook(GameEngine* This, uint32_t, L2ParamStack& stack)
	{
		EventDispatcher::GetInstance().Dispatch(ItemAutousedEvent{ stack.GetBufferAsVector<uint32_t>() });
		(*__OnExAutoSoulShot)(This, stack);
	}

	void __fastcall GameEngineWrapper::__Tick_hook(GameEngine* This, uint32_t, float_t deltaTime)
	{
		if (_target == 0)
		{
			_target = This;
		}

		(*__Tick)(This, deltaTime);

		EventDispatcher::GetInstance().Dispatch(GameEngineTickedEvent{});
	}
	void __fastcall GameEngineWrapper::__OnSay2_hook(GameEngine* This, uint32_t, L2ParamStack& stack)
	{
		const auto buffer = stack.GetBufferAsVector<uint32_t>();

		EventDispatcher::GetInstance().Dispatch(
			ChatMessageCreatedEvent
			{
				ChatMessage
				{
					buffer[0],
					static_cast<uint8_t>(buffer[1]),
					std::wstring(reinterpret_cast<wchar_t*>(buffer[2])),
					std::wstring(reinterpret_cast<wchar_t*>(buffer[3]))
				}
			}
		);

		(*__OnSay2)(This, stack);
	}
	void __fastcall GameEngineWrapper::__OnEndItemList_hook(GameEngine* This, uint32_t)
	{
		EventDispatcher::GetInstance().Dispatch(OnEndItemListEvent());
		(*__OnEndItemList)(This);
	}
	// TODO ini
	// 0 - фпс без ограничений
	float __fastcall GameEngineWrapper::__GetMaxTickRate_hook(GameEngine* This, int)
	{
		float fps = (*__GetMaxTickRate)(This);
		return 0.0f;
	}

	int __fastcall GameEngineWrapper::__OnDie_hook(GameEngine* This, int, User* creature, L2ParamStack& stack)
	{
		EventDispatcher::GetInstance().Dispatch(CreatureDiedEvent{ creature->objectId, stack.GetBufferAsVector<int32_t>() });

		return (*__OnDie)(This, creature, stack);
	}
}