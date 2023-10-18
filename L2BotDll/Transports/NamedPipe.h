#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <cstdint>
#include <format>
#include "Domain/Exceptions.h"
#include "Domain/Services/ServiceLocator.h"

#define BUFFER_SIZE 16384

class NamedPipe
{
public:
	const bool Connect(const std::wstring& pipeName)
	{
		if (m_Pipe == NULL || m_PipeName != pipeName)
		{
			if (m_Pipe != NULL) {
				DisconnectNamedPipe(m_Pipe);
				CloseHandle(m_Pipe);
			}
			else
			{
				CreateOverlapped(m_ConntectingOverlapped);
				CreateOverlapped(m_ReadingOverlapped);
				CreateOverlapped(m_WritingOverlapped);
			}

			m_Pipe = CreateNamedPipeW((L"\\\\.\\pipe\\" + pipeName).c_str(),
				PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				BUFFER_SIZE * sizeof(wchar_t),
				BUFFER_SIZE * sizeof(wchar_t),
				NMPWAIT_USE_DEFAULT_WAIT,
				NULL
			);

			if (m_Pipe == INVALID_HANDLE_VALUE)
			{
				throw RuntimeException(std::format(L"cannot create the pipe {}: {}", m_PipeName, GetLastError()));
			}
		}
		else
		{
			DisconnectNamedPipe(m_Pipe);
		}

		TryToConnect();

		WaitForMultipleObjects(1, &m_ConntectingOverlapped.hEvent, false, INFINITE);

		DWORD ret;
		m_Connected = GetOverlappedResult(m_Pipe, &m_ConntectingOverlapped, &ret, false);

		m_PipeName = pipeName;

		return m_Connected;
	}

	void Send(const std::wstring& message)
	{
		if (!m_Connected)
		{
			return;
		}

		const std::wstring preparedMessage = message + L"\n";

		DWORD written;
		const auto result = WriteFile(m_Pipe, message.c_str(), (message.size() + 1) * sizeof(wchar_t), &written, &m_WritingOverlapped);

		const auto lastError = GetLastError();
		if (!result)
		{
			if (lastError == ERROR_IO_PENDING)
			{
				WaitForMultipleObjects(1, &m_WritingOverlapped.hEvent, false, INFINITE);
				DWORD ret;
				const auto overlappedResult = GetOverlappedResult(m_Pipe, &m_WritingOverlapped, &ret, false);
				if (!overlappedResult)
				{
					m_Connected = false;
					throw RuntimeException(std::format(L"cannot get overlapped result for the pipe {} when writing", m_PipeName));
				}
			}
			else
			{
				m_Connected = false;
				throw RuntimeException(std::format(L"cannot write to the pipe {}: {}", m_PipeName, lastError));
			}
		}
	}

	const std::wstring Receive()
	{
		if (!m_Connected)
		{
			return L"";
		}

		DWORD dwRead;
		std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(BUFFER_SIZE);
		const auto result = ReadFile(m_Pipe, buffer.get(), BUFFER_SIZE * sizeof(wchar_t), &dwRead, &m_ReadingOverlapped);

		const auto lastError = GetLastError();
		if (!result)
		{
			if (lastError == ERROR_IO_PENDING)
			{
				WaitForMultipleObjects(1, &m_ReadingOverlapped.hEvent, false, INFINITE);
				DWORD ret;
				const auto overlappedResult = GetOverlappedResult(m_Pipe, &m_ReadingOverlapped, &ret, false);
				if (!overlappedResult)
				{
					m_Connected = false;
					throw RuntimeException(std::format(L"cannot get overlapped result for the pipe {} when reading", m_PipeName));
				}
			}
			else
			{
				m_Connected = false;
				throw RuntimeException(std::format(L"cannot read from the pipe {}: {}", m_PipeName, lastError));
			}
		}

		std::wstring message = std::wstring(buffer.get());

		return message;
	}

	const bool IsConnected() const
	{
		return m_Connected;
	}

	virtual ~NamedPipe()
	{
		if (m_Pipe != NULL)
		{
			CloseHandle(m_Pipe);
		}
	}
	NamedPipe() = default;

private:
	void TryToConnect()
	{
		const bool connected = ConnectNamedPipe(m_Pipe, &m_ConntectingOverlapped) == 0;
		if (!connected)
		{
			throw RuntimeException(std::format(L"cannot connect the pipe {}: {}", m_PipeName, GetLastError()));
		}

		switch (GetLastError())
		{
			// The overlapped connection in progress. 
		case ERROR_IO_PENDING:
			break;
			// Client is already connected, so signal an event. 
		case ERROR_PIPE_CONNECTED:
			if (SetEvent(m_ConntectingOverlapped.hEvent))
				break;
		default:
			throw RuntimeException(std::format(L"an error has occurred when connecting to the pipe ""{}"": {}", m_PipeName, GetLastError()));
		}
	}

	void CreateOverlapped(OVERLAPPED& overlapped)
	{
		if (overlapped.hEvent == NULL)
		{
			overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
			if (overlapped.hEvent == NULL)
			{
				throw RuntimeException(std::format(L"cannot create overlapped for the pipe {}: {}", m_PipeName, GetLastError()));
			}
			overlapped.Offset = 0;
			overlapped.OffsetHigh = 0;
		}
	}

private:
	std::wstring m_PipeName = L"";
	HANDLE m_Pipe = NULL;
	bool m_Connected = false;
	OVERLAPPED m_ConntectingOverlapped = OVERLAPPED();
	OVERLAPPED m_ReadingOverlapped = OVERLAPPED();
	OVERLAPPED m_WritingOverlapped = OVERLAPPED();
};