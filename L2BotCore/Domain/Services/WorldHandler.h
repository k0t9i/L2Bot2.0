#pragma once

#include <cstdint>
#include <thread>
#include <memory>
#include <Windows.h>
#include "../Serializers/SerializerInterface.h"
#include "../Repositories/EntityRepositoryInterface.h"
#include "../Transports/TransportInterface.h"
#include "../DTO/Message.h"
#include "../Services/IncomingMessageProcessor.h"
#include "../Services/OutgoingMessageBuilder.h"

namespace L2Bot::Domain::Services
{
	class WorldHandler
	{
	public:
		WorldHandler(
			const std::map<std::wstring, Repositories::EntityRepositoryInterface&> repositories,
			const Serializers::SerializerInterface& serializer,
			const Services::IncomingMessageProcessor& incomingMessageProcessor,
			Transports::TransportInterface& transport
		) :
			m_Repositories(repositories),
			m_Serializer(serializer),
			m_IncomingMessageProcessor(incomingMessageProcessor),
			m_Transport(transport)
		{

		}

		void Start()
		{
			for (const auto& kvp : m_Repositories)
			{
				kvp.second.Init();
			}

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
				const auto& messages = GetOutgoingMessages();

				if (m_Transport.IsConnected())
				{
					for (const auto& message : messages)
					{
						m_Transport.Send(
							m_Serializer.Serialize(message)
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
					const auto messageType = m_IncomingMessageProcessor.Process(m_Transport.Receive());

					if (messageType == Serializers::IncomingMessage::Type::invalidate) {
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

		const std::vector<std::vector<Serializers::Node>> GetOutgoingMessages()
		{
			std::vector<std::vector<Serializers::Node>> result;

			for (const auto& kvp : m_Repositories)
			{
				auto& entities = kvp.second.GetEntities();

				const auto& messages = m_OutgoingMessageBuilder.Build(kvp.first, entities);
				result.insert(result.end(), messages.begin(), messages.end());
			}

			return result;
		}

		void Invalidate()
		{
			for (const auto& kvp : m_Repositories)
			{
				kvp.second.Reset();
			}
		}

	private:
		const std::map<std::wstring, Repositories::EntityRepositoryInterface&> m_Repositories;
		const Serializers::SerializerInterface& m_Serializer;
		const Services::IncomingMessageProcessor m_IncomingMessageProcessor;
		Services::OutgoingMessageBuilder m_OutgoingMessageBuilder;

		Transports::TransportInterface& m_Transport;
		bool m_Stopped = false;
		std::thread m_ConnectingThread;
		std::thread m_SendingThread;
		std::thread m_ReceivingThread;
	};

}