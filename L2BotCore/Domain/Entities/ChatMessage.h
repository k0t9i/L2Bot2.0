#pragma once
#include <cstdint>
#include <string>
#include <functional>
#include "EntityInterface.h"
#include "../Enums/ChatChannelEnum.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class ChatMessage : public EntityInterface
	{
	public:
		const uint32_t GetId() const override
		{
			return m_Id;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint32_t>{}(m_Id),
				std::hash<uint32_t>{}(m_ObjectId),
				std::hash<Enums::ChatChannelEnum>{}(m_Channel),
				std::hash<std::wstring>{}(m_Name),
				std::hash<std::wstring>{}(m_Text)
			});
		}
		const std::string GetEntityName() const override
		{
			return "chat";
		}
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"objectId", std::to_wstring(m_ObjectId) },
				{ L"channel", std::to_wstring(static_cast<uint8_t>(m_Channel)) },
				{ L"name", m_Name },
				{ L"text", m_Text }
			};
		}

		ChatMessage(
			const uint32_t objectId,
			const Enums::ChatChannelEnum channel,
			const std::wstring& name,
			const std::wstring& text
		) :
			m_Id(++m_ChatGlobalId),
			m_ObjectId(objectId),
			m_Channel(channel),
			m_Name(name),
			m_Text(text)
		{
		}

		virtual ~ChatMessage() = default;
	private:
		uint32_t m_Id = 0;
		uint32_t m_ObjectId = 0;
		Enums::ChatChannelEnum m_Channel = Enums::ChatChannelEnum::all;
		std::wstring m_Name = L"";
		std::wstring m_Text = L"";
		static uint32_t m_ChatGlobalId;
	};
}
