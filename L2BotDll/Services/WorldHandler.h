#pragma once

#include <cstdint>
#include <thread>
#include <memory>
#include <Windows.h>
#include "Domain/Services/HeroServiceInterface.h"
#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Serializers/IncomingMessageFactoryInterface.h"
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/Transports/TransportInterface.h"
#include "Domain/DTO/Message.h"
#include "Domain/Services/UnitOfWork.h"

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
		Repositories::EntityRepositoryInterface& chatMessageRepository,
		const Serializers::SerializerInterface& serializer,
		const Serializers::IncomingMessageFactoryInterface& incomingMessageFactory,
		Services::HeroServiceInterface& heroService,
		Transports::TransportInterface& transport
	) :
		m_HeroRepository(heroRepository),
		m_DropRepository(dropRepository),
		m_NPCRepository(npcRepository),
		m_PlayerRepository(playerRepository),
		m_SkillRepository(skillRepository),
		m_ItemRepository(itemRepository),
		m_AbnormalEffectRepository(abnormalEffectRepository),
		m_ChatMessageRepository(chatMessageRepository),
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
		std::map<std::wstring, Repositories::EntityRepositoryInterface&> handlers
		{
			{L"hero", m_HeroRepository},
			{L"drop", m_DropRepository},
			{L"npc", m_NPCRepository},
			{L"player", m_PlayerRepository},
			{L"skill", m_SkillRepository},
			{L"item", m_ItemRepository},
			{L"abnormalEffect", m_AbnormalEffectRepository},
			{L"chat", m_ChatMessageRepository}
		};

		std::vector<std::vector<Serializers::Node>> result;

		for (const auto& kvp : handlers)
		{
			auto& entities = kvp.second.GetEntities();
			const auto& changes = m_UnitOfWork.ConnectEntities(kvp.first, entities);

			for (const auto &changeKvp : changes) {
				const auto id = changeKvp.first;

				std::wstring operation = L"none";
				switch (changeKvp.second)
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

				if (entities.find(id) != entities.end()) {
					result.push_back({
						Serializers::Node{ L"type", kvp.first },
						Serializers::Node{ L"operation", operation },
						Serializers::Node{ L"content", entities.at(id)->BuildSerializationNodes()}
					});
				}
				else {
					result.push_back({
						Serializers::Node{ L"type", kvp.first },
						Serializers::Node{ L"operation", operation },
						Serializers::Node{ L"content", {Serializers::Node{ L"id", std::to_wstring(id) }}}
					});
				}
			}
		}
		
		return result;
	}

	void Invalidate()
	{
		m_DropRepository.Reset();
		m_HeroRepository.Reset();
		m_NPCRepository.Reset();
		m_PlayerRepository.Reset();
		m_SkillRepository.Reset();
		m_ItemRepository.Reset();
		m_AbnormalEffectRepository.Reset();
	}

private:
	Repositories::EntityRepositoryInterface& m_DropRepository;
	Repositories::EntityRepositoryInterface& m_HeroRepository;
	Repositories::EntityRepositoryInterface& m_NPCRepository;
	Repositories::EntityRepositoryInterface& m_PlayerRepository;
	Repositories::EntityRepositoryInterface& m_SkillRepository;
	Repositories::EntityRepositoryInterface& m_ItemRepository;
	Repositories::EntityRepositoryInterface& m_AbnormalEffectRepository;
	Repositories::EntityRepositoryInterface& m_ChatMessageRepository;
	const Serializers::SerializerInterface& m_Serializer;
	const Serializers::IncomingMessageFactoryInterface& m_IncomingMessageFactory;
	Services::HeroServiceInterface& m_HeroService;
	Transports::TransportInterface& m_Transport;
	bool m_Stopped = false;
	std::thread m_ConnectingThread;
	std::thread m_SendingThread;
	std::thread m_ReceivingThread;

	Services::UnitOfWork m_UnitOfWork;
};
