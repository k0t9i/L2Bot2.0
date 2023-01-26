#pragma once
#pragma once

#include <cstdint>
#include <memory>
#include "../Entities/EntityInterface.h"
#include "../Enums/EntityStateEnum.h"

namespace L2Bot::Domain::DTO
{
	class Message : public Serializers::Serializable
	{
	public:
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::wstring operation = L"none";
			switch (m_Operation)
			{
			case Enums::EntityStateEnum::created:
				operation = L"create";
				break;
			case Enums::EntityStateEnum::updated:
				operation = L"update";
				break;
			case Enums::EntityStateEnum::deleted:
				operation = L"delete";
			}

			return
			{ 
				Serializers::Node{ L"type", m_Type },
				Serializers::Node{ L"operation", operation },
				Serializers::Node{L"content", m_Content.BuildSerializationNodes()}
			};
		}

		Message(const std::wstring& type, const Enums::EntityStateEnum operation, const Serializers::Serializable& content) :
			m_Type(type),
			m_Operation(operation),
			m_Content(content)
		{

		}

		Message() = default;
		virtual ~Message() = default;
	private:
		const std::wstring m_Type;
		const Enums::EntityStateEnum m_Operation;
		const Serializers::Serializable& m_Content;
	};
}