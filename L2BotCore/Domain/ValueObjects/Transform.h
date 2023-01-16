#pragma once
#include "../ValueObjects/Vector3.h"
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class Transform : public Serializers::Serializable
	{
	public:
		const Vector3& GetPosition() const
		{
			return m_Position;
		}
		const Vector3& GetRotation() const
		{
			return m_Rotation;
		}
		const Vector3& GetVelocity() const
		{
			return m_Velocity;
		}
		const Vector3& GetAcceleration() const
		{
			return m_Acceleration;
		}
		const bool IsEqual(const Transform* other) const
		{
			return m_Position.IsEqual(&other->m_Position) &&
				m_Rotation.IsEqual(&other->m_Rotation) &&
				m_Velocity.IsEqual(&other->m_Velocity) &&
				m_Acceleration.IsEqual(&other->m_Acceleration);
		}
		const float_t GetSqrDistance(const Transform& other) const
		{
			return m_Position.GetSqrDistance(other.m_Position);
		}
		const float_t GetHorizontalSqrDistance(const Transform& other) const
		{
			return m_Position.GetHorizontalSqrDistance(other.m_Position);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "position", m_Position.BuildSerializationNodes() },
				{ "rotation", m_Rotation.BuildSerializationNodes() },
				{ "velocity", m_Velocity.BuildSerializationNodes() },
				{ "acceleration", m_Acceleration.BuildSerializationNodes() }
			};
		}

		Transform(
			const Vector3 position,
			const Vector3 rotation,
			const Vector3 velocity,
			const Vector3 acceleration
		) :
			m_Position(position),
			m_Rotation(rotation),
			m_Velocity(velocity),
			m_Acceleration(acceleration)
		{
		}

		Transform() = default;
		virtual ~Transform() = default;
	private:
		Vector3 m_Position = Vector3();
		Vector3 m_Rotation = Vector3();
		Vector3 m_Velocity = Vector3();
		Vector3 m_Acceleration = Vector3();
	};
}
