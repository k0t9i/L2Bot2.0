#pragma once

#include <string>
#include "../Serializers/IncomingMessageFactoryInterface.h"
#include "HeroServiceInterface.h"
#include "../Enums/RestartPointTypeEnum.h"
#include "../DTO/UseSkillParams.h"

namespace L2Bot::Domain::Services
{
	class IncomingMessageProcessor
	{
	public:
		IncomingMessageProcessor(
			const Serializers::IncomingMessageFactoryInterface& factory,
			HeroServiceInterface& heroService
		) :
			m_Factory(factory),
			m_HeroService(heroService)
		{
		}
		virtual ~IncomingMessageProcessor() = default;

		const Serializers::IncomingMessage::Type Process(const std::wstring& incomingMessge) const
		{
			const auto message = m_Factory.CreateMessage(incomingMessge);
			switch (message.GetType())
			{
			case Serializers::IncomingMessage::Type::move:
				m_HeroService.Move(message.GetRawContent<ValueObjects::Vector3>());
				break;
			case Serializers::IncomingMessage::Type::acquireTarget:
				m_HeroService.AcquireTarget(message.GetRawContent<uint32_t>());
				break;
			case Serializers::IncomingMessage::Type::attack:
				m_HeroService.Attack(message.GetRawContent<uint32_t>());
				break;
			case Serializers::IncomingMessage::Type::pickup:
				m_HeroService.Pickup(message.GetRawContent<uint32_t>());
				break;
			case Serializers::IncomingMessage::Type::useSkill:
				const auto params = message.GetRawContent<DTO::UseSkillParams>();
				m_HeroService.UseSkill(params.skillId, params.isForced, params.isShiftPressed);
				break;
			case Serializers::IncomingMessage::Type::useItem:
				m_HeroService.UseItem(message.GetRawContent<uint32_t>());
				break;
			case Serializers::IncomingMessage::Type::toggleSoulshot:
				m_HeroService.ToggleAutouseSoulshot(message.GetRawContent<uint32_t>());
				break;
			case Serializers::IncomingMessage::Type::sit:
				m_HeroService.Sit();
				break;
			case Serializers::IncomingMessage::Type::stand:
				m_HeroService.Stand();
				break;
			case Serializers::IncomingMessage::Type::restartPoint:
				m_HeroService.RestartPoint(message.GetRawContent<Enums::RestartPointTypeEnum>());
				break;
			}

			return message.GetType();
		}

	private:
		const L2Bot::Domain::Serializers::IncomingMessageFactoryInterface& m_Factory;
		Services::HeroServiceInterface& m_HeroService;
	};
}