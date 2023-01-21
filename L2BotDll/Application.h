#pragma once

#include <Windows.h>
#include <string>

#include "Services/WorldHandler.h"
#include "Serializers/JsonSerializer.h"
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
			m_Serializer,
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
	NamedPipeTransport m_Transport;

	static const std::string PIPE_NAME;
	static const uint16_t RADIUS;
};

const std::string Application::PIPE_NAME = std::string("PipeL2Bot");
const uint16_t Application::RADIUS = 2000;