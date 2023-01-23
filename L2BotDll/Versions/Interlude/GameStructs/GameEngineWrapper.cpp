#include "pch.h"
#include "../../../Common/apihook.h"
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
#include "../../../DTO/ItemData.h"

namespace Interlude
{
	void* GameEngineWrapper::originalInitAddress = 0;
	GameEngineWrapper::GameEngine* GameEngineWrapper::_target = 0;

	void(__thiscall* GameEngineWrapper::__Init)(GameEngine*, float_t) = 0;
	void(__thiscall* GameEngineWrapper::__OnSkillListPacket)(GameEngine*, L2ParamStack&) = 0;
	int(__thiscall* GameEngineWrapper::__OnReceiveMagicSkillUse)(GameEngine*, User*, User*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__OnReceiveMagicSkillCanceled)(GameEngine*, User*) = 0;
	void(__thiscall* GameEngineWrapper::__AddAbnormalStatus)(GameEngine*, L2ParamStack&) = 0;
	void(__thiscall* GameEngineWrapper::__AddInventoryItem)(GameEngine*, ItemInfo&) = 0;
	void(__thiscall* GameEngineWrapper::__OnReceiveUpdateItemList)(GameEngine*, UpdateItemListActionType, ItemInfo&) = 0;


	void GameEngineWrapper::Init(HMODULE hModule)
	{
		void* initAddress = GetProcAddress(hModule, "?Tick@UGameEngine@@UAEXM@Z");
		originalInitAddress = splice(initAddress, __Init_hook);
		(FARPROC&)__Init = (FARPROC)initAddress;

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
	}

	void GameEngineWrapper::Restore()
	{
		restore(originalInitAddress);
		restore((void*&)__OnSkillListPacket);
		restore((void*&)__OnReceiveMagicSkillUse);
		restore((void*&)__OnReceiveMagicSkillCanceled);
		restore((void*&)__AddAbnormalStatus);
		restore((void*&)__AddInventoryItem);
		restore((void*&)__OnReceiveUpdateItemList);
	}

	void __fastcall GameEngineWrapper::__Init_hook(GameEngine* This, uint32_t /*edx*/, float_t unk)
	{
		if (_target == 0) {
			_target = This;

			InjectLibrary::StopCurrentProcess();
			restore(originalInitAddress);
			InjectLibrary::StartCurrentProcess();

			(*__Init)(This, unk);
		}
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

	void __fastcall GameEngineWrapper::__AddInventoryItem_hook(GameEngine* This, int, ItemInfo& itemInfo)
	{
		EventDispatcher::GetInstance().Dispatch(
			ItemCreatedEvent
			{
				ItemData
				{
					itemInfo.itemId,
					itemInfo.amount,
					itemInfo.isEquipped,
					itemInfo.enchantLevel,
					itemInfo.mana,
				}
			}
		);
		(*__AddInventoryItem)(This, itemInfo);
	}

	void __fastcall GameEngineWrapper::__OnReceiveUpdateItemList_hook(GameEngine* This, int, UpdateItemListActionType actionType, ItemInfo& itemInfo)
	{
		const ItemData itemData
		{
			itemInfo.itemId,
			itemInfo.amount,
			itemInfo.isEquipped,
			itemInfo.enchantLevel,
			itemInfo.mana,
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
			EventDispatcher::GetInstance().Dispatch(ItemDeletedEvent{ itemInfo.itemId });
			break;
		}
		(*__OnReceiveUpdateItemList)(This, actionType, itemInfo);
	}
}