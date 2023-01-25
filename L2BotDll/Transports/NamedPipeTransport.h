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
		OutputDebugStringW(m_PipeName.c_str());
		if (!m_ConnectionPipe.Connect(m_PipeName))
		{
			return false;
		}

		OutputDebugStringA("Client connected to connection pipe");

		const auto mainPipeName = GenerateUUID();

		m_ConnectionPipe.Send(L"\\\\.\\pipe\\" + mainPipeName);

		OutputDebugStringA("Name of main pipe sended");

		if (!m_Pipe.Connect(mainPipeName))
		{
			OutputDebugStringA(std::to_string(GetLastError()).c_str());
			return false;
		}
		OutputDebugStringA("Client connected to main pipe");

		m_Pipe.Send(L"Hello!");

		return true;
	}

	const void Send(const std::wstring& data) override
	{
		if (!m_Pipe.IsConnected())
		{
			return;
		}

		m_Pipe.Send(data);
	}
	const std::wstring Receive() override
	{
		if (!m_Pipe.IsConnected())
		{
			return L"";
		}

		return m_Pipe.Receive();
	}

	const bool IsConnected() const override
	{
		return m_Pipe.IsConnected();
	}

	NamedPipeTransport(const std::wstring& pipeName) :
		m_PipeName(pipeName)
	{
	}

	NamedPipeTransport() = delete;
	virtual ~NamedPipeTransport() = default;

private:
	NamedPipe m_ConnectionPipe;
	NamedPipe m_Pipe;
	std::wstring m_PipeName = L"";
};