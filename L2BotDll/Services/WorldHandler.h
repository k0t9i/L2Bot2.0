#pragma once

#include <cstdint>
#include <thread>
#include <memory>
#include <Windows.h>
#include "Domain/Services/EntityService.h"
#include "Domain/Serializers/SerializableStateContainer.h"
#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/Transports/TransportInterface.h"

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
		const Serializers::SerializerInterface& serializer,
		Transports::TransportInterface& transport
	) :
		m_HeroService(Services::EntityService(heroRepository)),
		m_DropService(Services::EntityService(dropRepository)),
		m_NPCService(Services::EntityService(npcRepository)),
		m_PlayerService(Services::EntityService(playerRepository)),
		m_SkillService(Services::EntityService(skillRepository)),
		m_ItemService(Services::EntityService(itemRepository)),
		m_Serializer(serializer),
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
						m_Serializer.Serialize({ item })
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
				const std::string& response = m_Transport.Receive();
				if (response == "invalidate")
				{
					Invalidate();
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

	const std::vector<Serializers::Node> GetData()
	{
		std::vector<Serializers::SerializableStateContainer> items
		{
			Serializers::SerializableStateContainer{m_HeroService.GetEntities(), "hero"},
			/*Serializers::SerializableStateContainer{m_DropService.GetEntities(), "drop"},
			Serializers::SerializableStateContainer{m_NPCService.GetEntities(), "npc"},
			Serializers::SerializableStateContainer{m_PlayerService.GetEntities(), "player"},
			Serializers::SerializableStateContainer{m_SkillService.GetEntities(), "skill"},*/
			Serializers::SerializableStateContainer{m_ItemService.GetEntities(), "item"},
		};

		std::vector<Serializers::Node> result;
		for (const auto& item : items)
		{
			for (const auto node : item.BuildSerializationNodes())
			{
				result.push_back(node);
			}
		}

		return result;
	}

	void Invalidate()
	{
		m_DropService.Invalidate();
		m_HeroService.Invalidate();
		m_NPCService.Invalidate();
		m_PlayerService.Invalidate();
		m_SkillService.Invalidate();
		m_ItemService.Invalidate();
	}

private:
	Services::EntityService m_DropService;
	Services::EntityService m_HeroService;
	Services::EntityService m_NPCService;
	Services::EntityService m_PlayerService;
	Services::EntityService m_SkillService;
	Services::EntityService m_ItemService;
	const Serializers::SerializerInterface& m_Serializer;
	Transports::TransportInterface& m_Transport;
	bool m_Stopped = false;
	std::thread m_ConnectingThread;
	std::thread m_SendingThread;
	std::thread m_ReceivingThread;
};
