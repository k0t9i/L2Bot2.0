#pragma once

#include "../VersionAbstractFactory.h"
#include "Factories/DropFactory.h"
#include "Factories/HeroFactory.h"
#include "Factories/NPCFactory.h"
#include "Factories/PlayerFactory.h"
#include "Factories/SkillFactory.h"
#include "Factories/ItemFactory.h"
#include "Factories/AbnormalEffectFactory.h"
#include "Factories/ChatMessageFactory.h"
#include "Repositories/DropRepository.h"
#include "Repositories/HeroRepository.h"
#include "Repositories/NPCRepository.h"
#include "Repositories/PlayerRepository.h"
#include "Repositories/SkillRepository.h"
#include "Repositories/ItemRepository.h"
#include "Repositories/AbnormalEffectRepository.h"
#include "Repositories/ChatMessageRepository.h"
#include "Services/HeroService.h"
#include "GameStructs/NetworkHandlerWrapper.h"
#include "GameStructs/GameEngineWrapper.h"
#include "GameStructs/L2GameDataWrapper.h"
#include "GameStructs/FName.h"
#include "../../Services/EntityFinder.h"
#include "Helpers/EnchantHelper.h"

namespace Interlude
{
	class AbstractFactory : public VersionAbstractFactory
	{
	public:
		AbstractFactory(const uint16_t radius) :
			m_Radius(radius)
		{

		}
		AbstractFactory() = delete;
		virtual ~AbstractFactory() = default;

		HeroRepository& GetHeroRepository() const override
		{
			static auto factory = HeroFactory();
			static EntityFinder finder;
			static auto result = HeroRepository(
				GetNetworkHandler(),
				factory,
				finder
			);
			return result;
		}
		DropRepository& GetDropRepository() const override
		{
			static auto factory = DropFactory(GetL2GameData(), GetFName());
			static EntityFinder finder;
			static auto result = DropRepository(
				GetNetworkHandler(),
				factory,
				finder,
				m_Radius
			);
			return result;
		}
		NPCRepository& GetNPCRepository() const override
		{
			static auto factory = NPCFactory();
			static EntityFinder finder;
			static auto result = NPCRepository(
				GetNetworkHandler(),
				factory,
				finder,
				m_Radius
			);
			return result;
		}
		PlayerRepository& GetPlayerRepository() const override
		{
			static auto factory = PlayerFactory();
			static EntityFinder finder;
			static auto result = PlayerRepository(
				GetNetworkHandler(),
				factory,
				finder,
				m_Radius
			);
			return result;
		}
		SkillRepository& GetSkillRepository() const override
		{
			static auto factory = SkillFactory(GetL2GameData(), GetFName());
			static EntityFinder finder;
			static auto result = SkillRepository(
				GetNetworkHandler(),
				factory,
				finder
			);
			return result;
		}
		ItemRepository& GetItemRepository() const override
		{
			static EnchantHelper enchantHelper;
			static auto factory = ItemFactory(GetL2GameData(), GetFName(), enchantHelper);
			static EntityFinder finder;
			static auto result = ItemRepository(
				GetNetworkHandler(),
				factory,
				finder
			);
			return result;
		}
		AbnormalEffectRepository& GetAbnormalEffectRepository() const override
		{
			static auto factory = AbnormalEffectFactory(GetL2GameData(), GetFName());
			static EntityFinder finder;
			static auto result = AbnormalEffectRepository(
				factory,
				finder
			);
			return result;
		}
		ChatMessageRepository& GetChatMessageRepository() const override
		{
			static auto factory = ChatMessageFactory();
			static auto result = ChatMessageRepository(
				factory
			);
			return result;
		}
		Services::HeroServiceInterface& GetHeroService() const override
		{
			static auto result = HeroService(
				GetNetworkHandler(),
				GetItemRepository(),
				GetL2GameData()
			);
			return result;
		}
		NetworkHandlerWrapper& GetNetworkHandler() const override
		{
			static NetworkHandlerWrapper result;
			return result;
		}
		GameEngineWrapper& GetGameEngine() const override
		{
			static GameEngineWrapper result;
			return result;
		}
		L2GameDataWrapper& GetL2GameData() const override
		{
			static L2GameDataWrapper result;
			return result;
		}
		FName& GetFName() const override
		{
			static FName result;
			return result;
		}

	private:
		const uint16_t m_Radius;
	};
}