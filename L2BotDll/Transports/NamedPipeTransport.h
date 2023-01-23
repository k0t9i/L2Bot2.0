#pragma once
#include "Domain/Transports/TransportInterface.h"
#include <Windows.h>
#include "NamedPipe.h"
#include "../Common/Common.h"

using namespace L2Bot::Domain;

class NamedPipeTransport : public Transports::TransportInterface
{
public:
	const bool Connect() override
	{
		OutputDebugStringA(m_PipeName.c_str());
		if (!m_ConnectionPipe.Connect(m_PipeName))
		{
			return false;
		}

		OutputDebugStringA("Client connected to connection pipe");

		const std::string mainPipeName = GenerateUUID();

		m_ConnectionPipe.Send("\\\\.\\pipe\\" + mainPipeName);

		OutputDebugStringA("Name of main pipe sended");

		if (!m_Pipe.Connect(mainPipeName))
		{
			OutputDebugStringA(std::to_string(GetLastError()).c_str());
			return false;
		}
		OutputDebugStringA("Client connected to main pipe");

		m_Pipe.Send("Hello!");

		return true;
	}

	const void Send(const std::string& data) override
	{
		if (!m_Pipe.IsConnected())
		{
			return;
		}

		m_Pipe.Send(data);
	}
	const std::string Receive() override
	{
		if (!m_Pipe.IsConnected())
		{
			return "";
		}

		return m_Pipe.Receive();
	}

	const bool IsConnected() const override
	{
		return m_Pipe.IsConnected();
	}

	NamedPipeTransport(const std::string& pipeName) :
		m_PipeName(pipeName)
	{
	}

	NamedPipeTransport() = delete;
	virtual ~NamedPipeTransport() = default;

private:
	NamedPipe m_ConnectionPipe;
	NamedPipe m_Pipe;
	std::string m_PipeName = "";
};