#pragma once

#include "../VersionAbstractFactory.h"
#include "Factories/HeroFactory.h"
#include "Factories/DropFactory.h"
#include "Factories/NPCFactory.h"
#include "Factories/PlayerFactory.h"
#include "Factories/SkillFactory.h"
#include "Repositories/HeroRepository.h"
#include "Repositories/DropRepository.h"
#include "Repositories/NPCRepository.h"
#include "Repositories/PlayerRepository.h"
#include "Repositories/SkillRepository.h"
#include "GameStructs/NetworkHandlerWrapper.h"
#include "GameStructs/GameEngineWrapper.h"
#include "GameStructs/L2GameDataWrapper.h"
#include "GameStructs/FName.h"

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
			static auto result = HeroRepository(
				GetNetworkHandler(),
				factory
			);
			return result;
		}
		DropRepository& GetDropRepository() const override
		{
			static auto factory = DropFactory(GetL2GameData(), GetFName());
			static auto result = DropRepository(
				GetNetworkHandler(),
				factory,
				m_Radius
			);
			return result;
		}
		NPCRepository& GetNPCRepository() const override
		{
			static auto factory = NPCFactory();
			static auto result = NPCRepository(GetNetworkHandler(), factory, m_Radius);
			return result;
		}
		PlayerRepository& GetPlayerRepository() const override
		{
			static auto factory = PlayerFactory();
			static auto result = PlayerRepository(GetNetworkHandler(), factory, m_Radius);
			return result;
		}
		SkillRepository& GetSkillRepository() const override
		{
			static auto factory = SkillFactory(GetL2GameData(), GetFName());
			static auto result = SkillRepository(GetNetworkHandler(), factory);
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