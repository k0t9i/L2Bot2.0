#pragma once
#include <functional>
#include "../Enums/RaceEnum.h"
#include "../Enums/ClassEnum.h"
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class Phenotype : public Serializers::Serializable, public Entities::Hashable
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
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<Enums::RaceEnum>{}(m_Race),
				std::hash<bool>{}(m_IsMale),
				std::hash<Enums::ClassEnum>{}(m_Class),
				std::hash<Enums::ClassEnum>{}(m_ActiveClass)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"race", std::to_wstring(static_cast<uint8_t>(m_Race)) },
				{ L"isMale", std::to_wstring(m_IsMale) },
				{ L"class", std::to_wstring(static_cast<uint8_t>(m_Class)) },
				{ L"activeClass", std::to_wstring(static_cast<uint8_t>(m_ActiveClass)) }
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
