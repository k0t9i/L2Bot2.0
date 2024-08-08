#pragma once

#include<map>
#include <string>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/Services/HeroServiceInterface.h"
#include "GameStructs/NetworkHandlerInterface.h"
#include "GameStructs/GameEngineInterface.h"
#include "GameStructs/L2GameDataInterface.h"
#include "GameStructs/FNameInterface.h"

using namespace L2Bot::Domain;

class VersionAbstractFactory
{
public:
	enum class Version
	{
		interlude
	};

	virtual Repositories::EntityRepositoryInterface& GetHeroRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetDropRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetNPCRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetPlayerRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetSkillRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetItemRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetAbnormalEffectRepository() const = 0;
	virtual Repositories::EntityRepositoryInterface& GetChatMessageRepository() const = 0;
	virtual Services::HeroServiceInterface& GetHeroService() const = 0;
	virtual NetworkHandlerInterface& GetNetworkHandler() const = 0;
	virtual GameEngineInterface& GetGameEngine() const = 0;
	virtual L2GameDataInterface& GetL2GameData() const = 0;
	virtual FNameInterface& GetFName() const = 0;

	virtual const std::map<std::wstring, Repositories::EntityRepositoryInterface&> GetRepositories() const
	{
		return {
			{L"hero", GetHeroRepository()},
			{L"drop", GetDropRepository()},
			{L"npc", GetNPCRepository()},
			{L"player", GetPlayerRepository()},
			{L"skill", GetSkillRepository()},
			{L"item", GetItemRepository()},
			//{L"abnormalEffect", GetAbnormalEffectRepository()},
			{L"chat", GetChatMessageRepository()}
		};
	}

	static const VersionAbstractFactory& GetFactory(Version version, const uint16_t radius);
};