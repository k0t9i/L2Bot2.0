#pragma once

#include "Domain/Repositories/HeroRepositoryInterface.h"
#include "Domain/Repositories/DropRepositoryInterface.h"
#include "Domain/Repositories/NPCRepositoryInterface.h"
#include "Domain/Repositories/PlayerRepositoryInterface.h"
#include "Domain/Repositories/SkillRepositoryInterface.h"
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

	virtual Repositories::HeroRepositoryInterface& GetHeroRepository() const = 0;
	virtual Repositories::DropRepositoryInterface& GetDropRepository() const = 0;
	virtual Repositories::NPCRepositoryInterface& GetNPCRepository() const = 0;
	virtual Repositories::PlayerRepositoryInterface& GetPlayerRepository() const = 0;
	virtual Repositories::SkillRepositoryInterface& GetSkillRepository() const = 0;
	virtual NetworkHandlerInterface& GetNetworkHandler() const = 0;
	virtual GameEngineInterface& GetGameEngine() const = 0;
	virtual L2GameDataInterface& GetL2GameData() const = 0;
	virtual FNameInterface& GetFName() const = 0;

	static const VersionAbstractFactory& GetFactory(Version version, const uint16_t radius);
};