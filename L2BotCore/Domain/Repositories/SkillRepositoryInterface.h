#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/Skill.h"

namespace L2Bot::Domain::Repositories
{
	class SkillRepositoryInterface : public ObjectRepositoryInterface<DTO::Skill>
	{
	public:
		virtual const std::map<uint32_t, DTO::Skill> GetObjects() override = 0;
	};
}
