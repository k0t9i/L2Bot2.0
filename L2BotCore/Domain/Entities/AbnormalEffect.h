#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "EntityInterface.h"

namespace L2Bot::Domain::Entities
{
	class AbnormalEffect : public EntityInterface
	{
	public:
		const uint32_t GetId() const override
		{
			return m_SkillId;
		}

		void Update(const EntityInterface* other) override
		{
			const AbnormalEffect* casted = static_cast<const AbnormalEffect*>(other);
			SaveState();

			m_SkillId = casted->m_SkillId;
			m_Level = casted->m_Level;
			m_Name = casted->m_Name;
			m_Description = casted->m_Description;
			m_IconName = casted->m_IconName;
		}
		void SaveState() override
		{
			m_IsNewState = false;
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const AbnormalEffect* casted = static_cast<const AbnormalEffect*>(other);
			return m_SkillId == casted->m_SkillId &&
				m_Level == casted->m_Level &&
				m_Name == casted->m_Name &&
				m_Description == casted->m_Description &&
				m_IconName == casted->m_IconName;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ "skillId", std::to_string(m_SkillId) });
			result.push_back({ "level", std::to_string(m_Level) });

			if (m_IsNewState)
			{
				result.push_back({ "name", m_Name });
				result.push_back({ "iconName", m_IconName });
				result.push_back({ "description", m_Description });
			}

			return result;
		}

		AbnormalEffect(
			const uint32_t skillId,
			const uint8_t level,
			const std::string& name,
			const std::string& description,
			const std::string& iconName
		) :
			m_SkillId(skillId),
			m_Level(level),
			m_Name(name),
			m_Description(description),
			m_IconName(iconName)
		{

		}

		AbnormalEffect(const AbnormalEffect* other) :
			m_SkillId(other->m_SkillId),
			m_Level(other->m_Level),
			m_Name(other->m_Name),
			m_Description(other->m_Description),
			m_IconName(other->m_IconName)
		{
		}

		AbnormalEffect() = default;
		virtual ~AbnormalEffect() = default;

	private:
		uint32_t m_SkillId = 0;
		uint8_t m_Level = 0;
		std::string m_Name = "";
		std::string m_Description = "";
		std::string m_IconName = "";
		bool m_IsNewState = true;
	};
}
