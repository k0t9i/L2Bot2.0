#pragma once

#include <Windows.h>
#include <string>

#include "Services/WorldHandler.h"
#include "Serializers/JsonSerializer.h"
#include "Serializers/JsonIncomingMessageFactory.h"
#include "Transports/NamedPipeTransport.h"

#include "Versions/VersionAbstractFactory.h"

using namespace L2Bot::Domain;

class Application
{
public:
	Application(const VersionAbstractFactory::Version version) :
		m_AbstractFactory(VersionAbstractFactory::GetFactory(version, Application::RADIUS)),
		m_Transport(Application::PIPE_NAME),
		m_WorldHandler
		(
			m_AbstractFactory.GetHeroRepository(),
			m_AbstractFactory.GetDropRepository(),
			m_AbstractFactory.GetNPCRepository(),
			m_AbstractFactory.GetPlayerRepository(),
			m_AbstractFactory.GetSkillRepository(),
			m_AbstractFactory.GetItemRepository(),
			m_AbstractFactory.GetAbnormalEffectRepository(),
			m_AbstractFactory.GetChatMessageRepository(),
			m_Serializer,
			m_MessageFactory,
			m_AbstractFactory.GetHeroService(),
			m_Transport
		)
	{

	}
	Application() = delete;
	virtual ~Application() = default;

	void Start()
	{
		HMODULE hEngine = GetModuleHandleA("Engine.dll");
		HMODULE hCore = GetModuleHandleA("Core.dll");

		m_AbstractFactory.GetNetworkHandler().Init(hEngine);
		m_AbstractFactory.GetGameEngine().Init(hEngine);
		m_AbstractFactory.GetL2GameData().Init(hEngine);
		m_AbstractFactory.GetFName().Init(hCore);
		m_WorldHandler.Start();
	}

	void Stop()
	{
		m_WorldHandler.Stop();
		m_AbstractFactory.GetL2GameData().Restore();
		m_AbstractFactory.GetGameEngine().Restore();
		m_AbstractFactory.GetNetworkHandler().Restore();
	}

private:
	const VersionAbstractFactory& m_AbstractFactory;
	WorldHandler m_WorldHandler;
	JsonSerializer m_Serializer;
	JsonIncomingMessageFactory m_MessageFactory;
	NamedPipeTransport m_Transport;

	static const std::wstring PIPE_NAME;
	static const uint16_t RADIUS;
};

const std::wstring Application::PIPE_NAME = std::wstring(L"PipeL2Bot");
const uint16_t Application::RADIUS = 2000;