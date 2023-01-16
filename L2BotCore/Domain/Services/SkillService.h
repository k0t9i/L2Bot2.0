#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/Skill.h"
#include "../ValueObjects/Skill.h"
#include "../Repositories/SkillRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class SkillService : public ObjectService<ValueObjects::Skill, DTO::Skill>
	{
	public:
		SkillService(Repositories::SkillRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~SkillService() override = default;
	};
}
