#pragma once

#include <stdexcept>
#include <string>
#include <stdlib.h>

namespace L2Bot::Domain
{
	class RuntimeException : public std::runtime_error
	{
	public:
		using _Mybase = std::runtime_error;

		explicit RuntimeException(const std::wstring& _Message) : _Mybase(convert(_Message)), m_Message(_Message) {}

		explicit RuntimeException(const wchar_t* _Message) : RuntimeException(std::wstring(_Message)) {}

		/// returns the explanatory string
		const char* what() const noexcept override
		{
			return std::runtime_error::what();
		}

		const std::wstring& Message() const noexcept
		{
			return m_Message;
		}

#if !_HAS_EXCEPTIONS
	protected:
		virtual void _Doraise() const override { // perform class-specific exception handling
			_RAISE(*this);
		}
#endif // !_HAS_EXCEPTIONS

	private:
		const std::string convert(const std::wstring& wmsg) const
		{
			const auto size = wmsg.size() + 1;
			char* msg = new char[size];
			size_t charsConverted = 0;
			wcstombs_s(&charsConverted, msg, size, wmsg.c_str(), wmsg.length());

			const std::string result(msg);
			delete[] msg;

			return result;
		}

	private:
		const std::wstring m_Message;
	};

	class CriticalRuntimeException : public RuntimeException
	{
	public:
		explicit CriticalRuntimeException(const std::wstring& _Message) : RuntimeException(_Message) {}

		explicit CriticalRuntimeException(const wchar_t* _Message) : RuntimeException(_Message) {}
	};

}