#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "EntityInterface.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class AbnormalEffect : public EntityInterface
	{
	public:
		const uint32_t GetId() const override
		{
			return m_SkillId;
		}

		void Update(
			const uint8_t level,
			const std::wstring& name,
			const std::wstring& description,
			const std::wstring& iconName
		) {
			m_Level = level;
			m_Name = name;
			m_Description = description;
			m_IconName = iconName;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint32_t>{}(m_SkillId),
				std::hash<uint8_t>{}(m_Level),
				std::hash<std::wstring>{}(m_Name),
				std::hash<std::wstring>{}(m_Description),
				std::hash<std::wstring>{}(m_IconName)
			});
		}
		const std::string GetEntityName() const override
		{
			return "abnormalEffect";
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(m_SkillId) });
			result.push_back({ L"level", std::to_wstring(m_Level) });
			result.push_back({ L"name", m_Name });
			result.push_back({ L"iconName", m_IconName });
			result.push_back({ L"description", m_Description });

			return result;
		}

		AbnormalEffect(
			const uint32_t skillId,
			const uint8_t level,
			const std::wstring& name,
			const std::wstring& description,
			const std::wstring& iconName
		) :
			m_SkillId(skillId),
			m_Level(level),
			m_Name(name),
			m_Description(description),
			m_IconName(iconName)
		{

		}

		AbnormalEffect() = default;
		virtual ~AbnormalEffect() = default;

	private:
		uint32_t m_SkillId = 0;
		uint8_t m_Level = 0;
		std::wstring m_Name = L"";
		std::wstring m_Description = L"";
		std::wstring m_IconName = L"";
	};
}
