#pragma once
#include <cstdint>
#include <thread>
#include <Windows.h>
#include "Domain/Services/DropService.h"
#include "Domain/Services/HeroService.h"
#include "Domain/Services/NPCService.h"
#include "Domain/Services/PlayerService.h"
#include "Domain/Services/SkillService.h"
#include "Domain/Serializers/SerializableStateContainer.h"
#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Repositories/DropRepositoryInterface.h"
#include "Domain/Repositories/SkillRepositoryInterface.h"
#include "Domain/Transports/TransportInterface.h"

using namespace L2Bot::Domain;

class WorldHandler
{
public:
	WorldHandler(
		Repositories::HeroRepositoryInterface& heroRepository,
		Repositories::DropRepositoryInterface& dropRepository,
		Repositories::NPCRepositoryInterface& npcRepository,
		Repositories::PlayerRepositoryInterface& playerRepository,
		Repositories::SkillRepositoryInterface& skillRepository,
		const Serializers::SerializerInterface& serializer,
		Transports::TransportInterface& transport
	) :
		m_DropService(Services::DropService(dropRepository)),
		m_HeroService(Services::HeroService(heroRepository)),
		m_NPCService(Services::NPCService(npcRepository)),
		m_PlayerService(Services::PlayerService(playerRepository)),
		m_SkillService(Services::SkillService(skillRepository)),
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
		std::vector<Serializers::Serializable*> items
		{
			new Serializers::SerializableStateContainer<Entities::Hero>(m_HeroService.GetObjects(), "hero"),
			new Serializers::SerializableStateContainer<Entities::Drop>(m_DropService.GetObjects(), "drop"),
			new Serializers::SerializableStateContainer<Entities::NPC>(m_NPCService.GetObjects(), "npc"),
			new Serializers::SerializableStateContainer<Entities::Player>(m_PlayerService.GetObjects(), "player"),
			new Serializers::SerializableStateContainer<ValueObjects::Skill>(m_SkillService.GetObjects(), "skill")
		};

		std::vector<Serializers::Node> result;
		for (const auto& item : items)
		{
			for (const auto node : item->BuildSerializationNodes())
			{
				result.push_back(node);
			}
		}

		for (const auto& item : items)
		{
			delete item;
		}

		return result;
	}

	void Invalidate()
	{
		m_HeroService.Invalidate();
		m_DropService.Invalidate();
		m_NPCService.Invalidate();
		m_PlayerService.Invalidate();
		m_SkillService.Invalidate();
	}

private:
	Services::DropService m_DropService;
	Services::HeroService m_HeroService;
	Services::NPCService m_NPCService;
	Services::PlayerService m_PlayerService;
	Services::SkillService m_SkillService;
	const Serializers::SerializerInterface& m_Serializer;
	Transports::TransportInterface& m_Transport;
	bool m_Stopped = false;
	std::thread m_ConnectingThread;
	std::thread m_SendingThread;
	std::thread m_ReceivingThread;
};
