#pragma once
#include "../Enums/RaceEnum.h"
#include "../Enums/ClassEnum.h"
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class Phenotype : public Serializers::Serializable
	{
	public:
		const bool IsSubClass() const
		{
			return m_ActiveClass != Enums::ClassEnum::none && m_Class != m_ActiveClass;
		}
		const Enums::RaceEnum GetRace() const
		{
			return m_Race;
		}
		const bool IsMale() const
		{
			return m_IsMale;
		}
		const Enums::ClassEnum GetClass() const
		{
			return m_Class;
		}
		const Enums::ClassEnum GetActiveClass() const
		{
			return m_ActiveClass;
		}
		const bool IsEqual(const Phenotype* other) const
		{
			return m_Race == other->m_Race &&
				m_IsMale == other->m_IsMale &&
				m_Class == other->m_Class &&
				m_ActiveClass == other->m_ActiveClass;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "race", std::to_string(static_cast<uint8_t>(m_Race)) },
				{ "isMale", std::to_string(m_IsMale) },
				{ "class", std::to_string(static_cast<uint8_t>(m_Class)) },
				{ "activeClass", std::to_string(static_cast<uint8_t>(m_ActiveClass)) }
			};
		}

		Phenotype(
			Enums::RaceEnum race,
			bool isMale,
			Enums::ClassEnum class_,
			Enums::ClassEnum activeClass
		) :
			m_Race(race),
			m_IsMale(isMale),
			m_Class(class_),
			m_ActiveClass(activeClass)
		{
		}

		Phenotype() = default;
		virtual ~Phenotype() = default;

	private:
		Enums::RaceEnum m_Race = Enums::RaceEnum::none;
		bool m_IsMale = true;
		Enums::ClassEnum m_Class = Enums::ClassEnum::none;
		Enums::ClassEnum m_ActiveClass = Enums::ClassEnum::none;
	};
}
