#pragma once

#include <memory>

namespace L2Bot::Domain::Serializers
{
	class IncomingMessage
	{
	public:
		enum class Type
		{
			none,
			invalidate,
			move,
			acquireTarget,
			attack,
			pickup,
			useSkill,
			useItem,
			toggleSoulshot,
			sit,
			stand
		};

		const Type GetType() const
		{
			return m_Type;
		}

		template <typename T>
		const auto GetContent() const
		{
			return std::static_pointer_cast<T>(m_Content);
		}

		template <typename T>
		const T GetRawContent() const
		{
			return *std::static_pointer_cast<T>(m_Content).get();
		}

		IncomingMessage(Type type, std::shared_ptr<void> content) :
			m_Type(type),
			m_Content(content)
		{

		}

		IncomingMessage(Type type) :
			m_Type(type)
		{

		}

		IncomingMessage() = default;
		virtual ~IncomingMessage() = default;

	private:
		Type m_Type = Type::none;
		std::shared_ptr<void> m_Content;
	};
}
