#pragma once
#include <cstdint>
#include <string>
#include "../Serializers/Serializable.h"
#include "../Enums/ChatChannelEnum.h"

namespace L2Bot::Domain::ValueObjects
{
	class ChatMessage : public Serializers::Serializable
	{
	public:
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "objectId", std::to_string(m_ObjectId) },
				{ "channel", std::to_string(static_cast<uint8_t>(m_Channel)) },
				{ "name", m_Name },
				{ "text", m_Text }
			};
		}

		ChatMessage(
			const uint32_t objectId,
			const Enums::ChatChannelEnum channel,
			const std::string& name,
			const std::string& text
		) :
			m_ObjectId(objectId),
			m_Channel(channel),
			m_Name(name),
			m_Text(text)
		{

		}

		ChatMessage() = default;
		virtual ~ChatMessage() = default;
	private:
		uint32_t m_ObjectId = 0;
		Enums::ChatChannelEnum m_Channel = Enums::ChatChannelEnum::all;
		std::string m_Name = "";
		std::string m_Text = "";
	};
}
