#pragma once

#include <cstdint>
#include <thread>
#include <memory>
#include <Windows.h>
#include "Domain/Services/EntityHandler.h"
#include "Domain/Services/ChatMessageHandler.h"
#include "Domain/Services/HeroServiceInterface.h"
#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Serializers/IncomingMessageFactoryInterface.h"
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/Transports/TransportInterface.h"
#include "Domain/DTO/Message.h"

using namespace L2Bot::Domain;

class WorldHandler
{
public:
	WorldHandler(
		Repositories::EntityRepositoryInterface& heroRepository,
		Repositories::EntityRepositoryInterface& dropRepository,
		Repositories::EntityRepositoryInterface& npcRepository,
		Repositories::EntityRepositoryInterface& playerRepository,
		Repositories::EntityRepositoryInterface& skillRepository,
		Repositories::EntityRepositoryInterface& itemRepository,
		Repositories::EntityRepositoryInterface& abnormalEffectRepository,
		Repositories::ChatMessageRepositoryInterface& chatMessageRepository,
		const Serializers::SerializerInterface& serializer,
		const Serializers::IncomingMessageFactoryInterface& incomingMessageFactory,
		Services::HeroServiceInterface& heroService,
		Transports::TransportInterface& transport
	) :
		m_HeroHandler(Services::EntityHandler(heroRepository)),
		m_DropHandler(Services::EntityHandler(dropRepository)),
		m_NPCHandler(Services::EntityHandler(npcRepository)),
		m_PlayerHandler(Services::EntityHandler(playerRepository)),
		m_SkillHandler(Services::EntityHandler(skillRepository)),
		m_ItemHandler(Services::EntityHandler(itemRepository)),
		m_AbnormalEffectHandler(Services::EntityHandler(abnormalEffectRepository)),
		m_ChatMessageHandler(Services::ChatMessageHandler(chatMessageRepository)),
		m_Serializer(serializer),
		m_IncomingMessageFactory(incomingMessageFactory),
		m_HeroService(heroService),
		m_Transport(transport)
	{

	}

	void Start()
	{
		m_ConnectingThread = std::thread(&WorldHandler::Connect, this);
		m_SendingThread = std::thread(&WorldHandler::Send, this);
		m_ReceivingThread = std::thread(&WorldHandler::Receive, this);
	}

	void Stop()
	{
		m_Stopped = true;
		if (m_ConnectingThread.joinable())
		{
			m_ConnectingThread.join();
		}
		if (m_SendingThread.joinable())
		{
			m_SendingThread.join();
		}
		if (m_ReceivingThread.joinable())
		{
			m_ReceivingThread.join();
		}
	}

	virtual ~WorldHandler()
	{
		Stop();
	}

private:
	void Send()
	{
		while (!m_Stopped)
		{
			const auto& data = GetData();

			if (m_Transport.IsConnected())
			{
				for (const auto& item : data)
				{
					m_Transport.Send(
						m_Serializer.Serialize(item)
					);
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void Receive()
	{
		while (!m_Stopped)
		{
			if (m_Transport.IsConnected())
			{
				const auto message = m_IncomingMessageFactory.CreateMessage(m_Transport.Receive());
				switch (message.GetType())
				{
				case Serializers::IncomingMessage::Type::invalidate:
					Invalidate();
					break;
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
					m_HeroService.UseSkill(message.GetRawContent<uint32_t>(), false, false);
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
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void Connect()
	{
		while (!m_Stopped)
		{
			if (!m_Transport.IsConnected())
			{
				m_Transport.Connect();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	const std::vector<std::vector<Serializers::Node>> GetData()
	{
		std::map<std::wstring, Services::EntityHandler> handlers
		{
			{L"hero", m_HeroHandler},
			{L"drop", m_DropHandler},
			{L"npc", m_NPCHandler},
			{L"player", m_PlayerHandler},
			{L"skill", m_SkillHandler},
			{L"item", m_ItemHandler},
			{L"abnormalEffect", m_AbnormalEffectHandler}
		};

		std::vector<std::vector<Serializers::Node>> result;

		for (auto& kvp : handlers)
		{
			for (const auto& entity : kvp.second.GetEntities())
			{
				if (entity->GetState() != Enums::EntityStateEnum::none)
				{
					const auto message = DTO::Message{ kvp.first, entity->GetState(), *entity->GetEntity().get() };
					result.push_back(message.BuildSerializationNodes());
				}
			};
		}
		for (const auto& chatMessage : m_ChatMessageHandler.GetMessages())
		{
			const auto message = DTO::Message{ L"chat", Enums::EntityStateEnum::created, chatMessage };
			result.push_back(message.BuildSerializationNodes());
		}
		
		return result;
	}

	void Invalidate()
	{
		m_DropHandler.Invalidate();
		m_HeroHandler.Invalidate();
		m_NPCHandler.Invalidate();
		m_PlayerHandler.Invalidate();
		m_SkillHandler.Invalidate();
		m_ItemHandler.Invalidate();
		m_AbnormalEffectHandler.Invalidate();
	}

private:
	Services::EntityHandler m_DropHandler;
	Services::EntityHandler m_HeroHandler;
	Services::EntityHandler m_NPCHandler;
	Services::EntityHandler m_PlayerHandler;
	Services::EntityHandler m_SkillHandler;
	Services::EntityHandler m_ItemHandler;
	Services::EntityHandler m_AbnormalEffectHandler;
	Services::ChatMessageHandler m_ChatMessageHandler;
	const Serializers::SerializerInterface& m_Serializer;
	const Serializers::IncomingMessageFactoryInterface& m_IncomingMessageFactory;
	Services::HeroServiceInterface& m_HeroService;
	Transports::TransportInterface& m_Transport;
	bool m_Stopped = false;
	std::thread m_ConnectingThread;
	std::thread m_SendingThread;
	std::thread m_ReceivingThread;
};
