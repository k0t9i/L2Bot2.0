#pragma once

#include "Domain/Serializers/IncomingMessageFactoryInterface.h"
#include "../ThirdParty/json.hpp"
#include "Domain/ValueObjects/Vector3.h"
#include "Domain/Enums/RestartPointTypeEnum.h"

using namespace L2Bot::Domain;
using json = nlohmann::json;

class JsonIncomingMessageFactory : public Serializers::IncomingMessageFactoryInterface
{
public:
	JsonIncomingMessageFactory() = default;
	virtual ~JsonIncomingMessageFactory() = default;

	const Serializers::IncomingMessage CreateMessage(std::wstring data) const override
	{
		try
		{
			auto jsonObject = json::parse(data);
			const auto type = jsonObject["Type"].get<std::string>();

			return CreateConcreteMessage(type, jsonObject["Content"]);
		}
		catch (json::exception& e)
		{
			return Serializers::IncomingMessage();
		}
	}

private:
	const Serializers::IncomingMessage CreateConcreteMessage(const std::string& type, const json& jsonObject) const
	{
		if (type == "Invalidate")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::invalidate
			};
		}
		else if (type == "Move")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::move,
				std::make_shared<ValueObjects::Vector3>(
					jsonObject["X"].get<float>(),
					jsonObject["Y"].get<float>(),
					jsonObject["Z"].get<float>()
				)
			};
		}
		else if (type == "AcquireTarget")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::acquireTarget,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "Attack")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::attack,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "Pickup")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::pickup,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "UseSkill")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::useSkill,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "UseItem")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::useItem,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "ToggleSoulshot")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::toggleSoulshot,
				std::make_shared<uint32_t>(jsonObject.get<uint32_t>())
			};
		}
		else if (type == "Sit")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::sit
			};
		}
		else if (type == "Stand")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::stand
			};
		}
		else if (type == "RestartPoint")
		{
			return Serializers::IncomingMessage
			{
				Serializers::IncomingMessage::Type::restartPoint,
				std::make_shared<Enums::RestartPointTypeEnum>(jsonObject.get<Enums::RestartPointTypeEnum>())
			};
		}

		return Serializers::IncomingMessage();
	}
};