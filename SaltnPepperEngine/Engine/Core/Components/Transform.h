#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "cereal/cereal.hpp"

namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform
		{

		protected:

			// The local position of the Transform
			Vector3 m_position;

			//The local Rotation in Quaternion to overcome "Gimble Lock"
			Quaternion m_rotation;

			// The Local Scale of the transform
			Vector3 m_scale;

			// The world Matrix for the Transform / or th Parent Matrix
			mutable Matrix4 m_worldMatrix = Matrix4(1.0f);

			mutable Matrix4 m_normalMatrix = Matrix4(1.0f);

			void UpdateMatrix(Matrix4& mat) const;

		public:


			Transform();
			~Transform();

			// ================ GETTERS ========================

			// Get the world Position of the object
			const Vector3& GetPosition() const;

			// Get the world Rotation of the Object in Quaternion
			const Quaternion& GetRotation() const;

			// Get the world Rotation in Euler angles
			const Vector3 GetEulerRotation() const;

			// Gets the Local Scale
			const Vector3 GetScale() const;


			const Vector3 GetWorldPosition();
			const Quaternion GetWorldRotation();
			


			Matrix4& GetMatrix();
			Matrix4 GetLocalMatrix();
			Matrix4& GetNormalMatrix();

			void SetMatrix(const Matrix4& mat);
			void SetLocalMatrix(const Matrix4& mat);
		

			// ================= SETTERS ========================

			// Sets the local position
			void SetPosition(const Vector3& newPosition);

			// Sets the local Rotation in Quaternion
			void SetRotation(const Quaternion& newRot);

			// Sets the rotation in Degrees
			void SetEularRotation(const Vector3& eularRot);

			void SetScale(const Vector3& newScale);

			Vector3 GetUpVector();

			Vector3 GetRightVector();

			Vector3 GetForwardVector();

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("Position", m_position), cereal::make_nvp("Rotation", m_rotation), cereal::make_nvp("Scale", m_scale));
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("Position", m_position), cereal::make_nvp("Rotation", m_rotation), cereal::make_nvp("Scale", m_scale));
			}

		};
	}
}

#endif // !TRANSFORM_H

