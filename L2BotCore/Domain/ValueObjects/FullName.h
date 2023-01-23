#pragma once
#include <string>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class FullName : public Serializers::Serializable
	{
	public:
		const std::string& GetNickname() const
		{
			return m_Nickname;
		}
		const std::string& GetTitle() const
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
				{ "nickname", m_Nickname },
				{ "title", m_Title }
			};
		}

		FullName(
			const std::string& nickname,
			const std::string& title
		) :
			m_Nickname(nickname),
			m_Title(title)
		{
		}

		FullName() = default;
		virtual ~FullName() = default;
	private:
		std::string m_Nickname = "";
		std::string m_Title = "";
	};
}
