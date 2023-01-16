#pragma once

#include <Windows.h>
#include <string>
#include <cstdint>

#define BUFFER_SIZE 16384

class NamedPipe
{
public:
	const bool Connect(const std::string& pipeName)
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

			m_Pipe = CreateNamedPipeA(("\\\\.\\pipe\\" + pipeName).c_str(),
				PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				BUFFER_SIZE * sizeof(char),
				BUFFER_SIZE * sizeof(char),
				NMPWAIT_USE_DEFAULT_WAIT,
				NULL
			);

			if (m_Pipe == INVALID_HANDLE_VALUE)
			{
				OutputDebugStringA(std::to_string(GetLastError()).c_str());
				return false;
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

	void Send(const std::string& message)
	{
		if (!m_Connected)
		{
			return;
		}

		DWORD written;
		const auto result = WriteFile(m_Pipe, message.c_str(), message.size() + 1, &written, &m_WritingOverlapped);

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
				}
			}
			else
			{
				m_Connected = false;
			}
		}
	}

	const std::string Receive()
	{
		if (!m_Connected)
		{
			return "";
		}

		DWORD dwRead;
		char* buffer = new char[BUFFER_SIZE];
		const auto result = ReadFile(m_Pipe, buffer, BUFFER_SIZE * sizeof(char), &dwRead, &m_ReadingOverlapped);

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
					delete[] buffer;
					m_Connected = false;
					return "";
				}
			}
			else
			{
				delete[] buffer;
				m_Connected = false;
				return "";
			}
		}

		std::string message = std::string(buffer);
		delete[] buffer;

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
			OutputDebugStringA(std::to_string(GetLastError()).c_str());
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

			// If an error occurs during the connect operation... 
		default:
			OutputDebugStringA(std::to_string(GetLastError()).c_str());
		}
	}

	void CreateOverlapped(OVERLAPPED& overlapped)
	{
		if (overlapped.hEvent == NULL)
		{
			overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
			if (overlapped.hEvent == NULL)
			{
				OutputDebugStringA(std::to_string(GetLastError()).c_str());
				return;
			}
			overlapped.Offset = 0;
			overlapped.OffsetHigh = 0;
		}
	}

private:
	std::string m_PipeName = "";
	HANDLE m_Pipe = NULL;
	bool m_Connected = false;
	OVERLAPPED m_ConntectingOverlapped = OVERLAPPED();
	OVERLAPPED m_ReadingOverlapped = OVERLAPPED();
	OVERLAPPED m_WritingOverlapped = OVERLAPPED();
};