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
		std::wstring m_Name = L"";
		std::wstring m_Text = L"";
	};
}
