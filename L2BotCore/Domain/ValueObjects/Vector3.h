#pragma once
#include <math.h>
#include <functional>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class Vector3 : public Serializers::Serializable, public Entities::Hashable
	{
	public:
		const float_t GetX() const
		{
			return m_X;
		}
		const float_t GetY() const
		{
			return m_Y;
		}
		const float_t GetZ() const
		{
			return m_Z;
		}
		const size_t GetHash() const override
		{
			const auto x = std::round(m_X * 10000.0f) / 10000.0f;
			const auto y = std::round(m_Y * 10000.0f) / 10000.0f;
			const auto z = std::round(m_Z * 10000.0f) / 10000.0f;

			return Helpers::CombineHashes({
				std::hash<float_t>{}(m_X),
				std::hash<float_t>{}(m_Y),
				std::hash<float_t>{}(m_Z)
			});
		}
		const float_t GetSqrDistance(const Vector3& other) const
		{
			return (m_X - other.m_X) * (m_X - other.m_X) + 
				(m_Y - other.m_Y) * (m_Y - other.m_Y) +
				(m_Z - other.m_Z) * (m_Z - other.m_Z);
		}
		const float_t GetHorizontalSqrDistance(const Vector3& other) const
		{
			return (m_X - other.m_X) * (m_X - other.m_X) +
				(m_Y - other.m_Y) * (m_Y - other.m_Y);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"x", std::to_wstring(m_X) },
				{ L"y", std::to_wstring(m_Y) },
				{ L"z", std::to_wstring(m_Z) }
			};
		}

		Vector3(const float_t x, const float_t y, const float_t z) :
			m_X(x), m_Y(y), m_Z(z)
		{
		}

		Vector3() = default;
		virtual ~Vector3() = default;

	private:
		float_t m_X = 0;
		float_t m_Y = 0;
		float_t m_Z = 0;
	};
}