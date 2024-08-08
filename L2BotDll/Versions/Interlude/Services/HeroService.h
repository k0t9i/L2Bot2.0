#pragma once

#include "Domain/Services/HeroServiceInterface.h"
#include "../Repositories/ItemRepository.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../GameStructs/L2GameDataWrapper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class HeroService : public Services::HeroServiceInterface
	{
	public:
		HeroService(const NetworkHandlerWrapper& networkHandler, const ItemRepository& itemRespository, const L2GameDataWrapper& l2GameData) :
			m_NetworkHandler(networkHandler),
			m_ItemRespository(itemRespository),
			m_L2GameData(l2GameData)
		{

		}
		HeroService() = delete;
		virtual ~HeroService() = default;

		void Move(ValueObjects::Vector3 location) const override
		{
			auto hero = m_NetworkHandler.GetHero();
			
			if (hero) {
				m_NetworkHandler.MTL(
					hero->pawn,
					{ location.GetX(), location.GetY(), location.GetZ() },
					hero->pawn->Location,
					hero->pawn->terrainInfo,
					0
				);
			}
		}

		void AcquireTarget(int objectId) const override
		{
			auto target = m_NetworkHandler.GetUser(objectId);

			if (target) {
				auto currentTargetId = 0;
				auto hero = m_NetworkHandler.GetHero();
				if (hero && hero->pawn && hero->pawn->lineagePlayerController) {
					currentTargetId = hero->pawn->lineagePlayerController->targetObjectId;
				}
				if (currentTargetId != objectId) {
					m_NetworkHandler.Action(objectId, target->pawn->Location, 0);
				}
			}
		}

		void Attack(int objectId) const override
		{
			auto target = m_NetworkHandler.GetUser(objectId);

			if (target) {
				// Acquire target
				m_NetworkHandler.Action(objectId, target->pawn->Location, 0);
				// Attack
				m_NetworkHandler.Action(objectId, target->pawn->Location, 0);
			}
		}

		void Pickup(int objectId) const override
		{
			auto target = m_NetworkHandler.GetItem(objectId);

			if (target) {
				m_NetworkHandler.Action(objectId, target->pawn->Location, 0);
			}
		}

		void UseSkill(int skillId, bool isForced, bool isShiftPressed) const override
		{
			L2ParamStack* stack = new L2ParamStack(3);
			stack->PushBack((void*)skillId);
			stack->PushBack((void*)(isForced ? 1 : 0));
			stack->PushBack((void*)(isShiftPressed ? 1 : 0));

			m_NetworkHandler.RequestMagicSkillUse(*stack);

			delete stack;
		}

		void UseItem(int objectId) const override
		{
			L2ParamStack* stack = new L2ParamStack(1);
			stack->PushBack((void*)objectId);

			m_NetworkHandler.RequestUseItem(*stack);

			delete stack;
		}

		void ToggleAutouseSoulshot(int objectId) const override
		{
			const auto item = m_ItemRespository.GetItem(objectId);
			if (item)
			{
				const auto etcItem = static_cast<const Entities::EtcItem*>(item.get());

				L2ParamStack* stack = new L2ParamStack(2);
				stack->PushBack((void*)etcItem->GetItemId());
				stack->PushBack((void*)(etcItem->IsAutoused() ? 0 : 1));

				m_NetworkHandler.RequestAutoSoulShot(*stack);

				delete stack;
			}
		}

		void Sit() const override
		{
			m_NetworkHandler.ChangeWaitType(0);
		}

		void Stand() const override
		{
			m_NetworkHandler.ChangeWaitType(1);
		}

	private:
		const NetworkHandlerWrapper& m_NetworkHandler;
		const ItemRepository& m_ItemRespository;
		const L2GameDataWrapper& m_L2GameData;
	};
}