#pragma once
#include <string>
#include <functional>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class FullName : public Serializers::Serializable, public Entities::Hashable
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
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<std::wstring>{}(m_Nickname),
				std::hash<std::wstring>{}(m_Title)
			});
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
