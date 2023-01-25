#pragma once
#include <string>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class FullName : public Serializers::Serializable
	{
	public:
		const std::wstring& GetNickname() const
		{
			return m_Nickname;
		}
		const std::wstring& GetTitle() const
		{
			return m_Title;
		}
		const bool IsEqual(const FullName* other) const
		{
			return m_Nickname == other->m_Nickname && m_Title == other->m_Title;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"nickname", m_Nickname },
				{ L"title", m_Title }
			};
		}

		FullName(
			const std::wstring& nickname,
			const std::wstring& title
		) :
			m_Nickname(nickname),
			m_Title(title)
		{
		}

		FullName() = default;
		virtual ~FullName() = default;
	private:
		std::wstring m_Nickname = L"";
		std::wstring m_Title = L"";
	};
}
