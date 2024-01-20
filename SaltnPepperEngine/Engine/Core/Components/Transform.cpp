#include "Transform.h"

namespace SaltnPepperEngine
{
	namespace Components
	{
		Transform::Transform()
			: m_position(Vector3(0.0f))
			, m_rotation(Quaternion())
			, m_scale(Vector3(1.0f))
			, m_worldMatrix(Matrix4(1.0f))
		{
			UpdateMatrix(m_worldMatrix);
	
		}

		Transform::~Transform()
		{

		}

		const Vector3& Transform::GetPosition() const
		{
			return m_position;
		}
		const Quaternion& Transform::GetRotation() const
		{
			return m_rotation;
		}
		const Vector3 Transform::GetEulerRotation() const
		{
			Vector3 DegreeRotation = GetEularAnglesDegrees(m_rotation);

			return DegreeRotation;
		}

		void Transform::UpdateMatrix(Matrix4& mat) const
		{
			Matrix4 matModel = Matrix4(1.0f);

			mat = Math::Translate(matModel, m_position) * Math::QuatToMatrix(m_rotation) * Math::Scale(matModel, m_scale);

		}



		Matrix4& Transform::GetMatrix()
		{
			return m_worldMatrix;
		}

		Matrix4 Transform::GetLocalMatrix()
		{
			Matrix4 localMat = Matrix4(1.0f);
			UpdateMatrix(localMat);

			return localMat;
		}

		Matrix4& Transform::GetNormalMatrix()
		{
			Matrix4 localMat = GetLocalMatrix();
			m_normalMatrix = Transpose(Inverse(localMat));

			return m_normalMatrix;

		}

		void Transform::SetMatrix(const Matrix4& mat)
		{
			m_worldMatrix = mat * GetLocalMatrix();
			//m_worldMatrix = mat;

		}

		const Vector3 Transform::GetScale() const
		{
			return m_scale;
		}

		const Vector3 Transform::GetWorldPosition()
		{
			return m_worldMatrix[3];
		}

		const Quaternion Transform::GetWorldRotation()
		{
			return GetQuaternion(m_worldMatrix);
		}


		void Transform::SetPosition(const Vector3& newPosition)
		{
			m_position = newPosition;
		}

		

		void Transform::SetRotation(const Quaternion& newRot)
		{

			m_rotation = newRot;

		}

		void Transform::SetEularRotation(const Vector3& eularRot)
		{
			Vector3 radianRotation = Vector3(eularRot.x * DEGtoRAD, eularRot.y * DEGtoRAD, eularRot.z * DEGtoRAD);
			m_rotation = Quaternion(radianRotation);
			//m_rotation = Quaternion(Vector3(Radians(eularRot.x), Radians(eularRot.y), Radians(eularRot.z)));
		}



		void Transform::SetScale(const Vector3& newScale)
		{
			m_scale = newScale;
		}


		Vector3 Transform::GetUpVector()
		{
			Vector3 newUp = Vector3(0.0f, 1.0f, 0.0f);

			return (GetRotation() * newUp);
		}

		Vector3 Transform::GetRightVector()
		{
			Vector3 newRight = Vector3(1.0f, 0.0f, 0.0f);

			return (GetRotation() * newRight);
		}

		Vector3 Transform::GetForwardVector()
		{
			// Open GL is stupid and has -z as forward
			Vector3 newForward = Vector3(0.0f, 0.0f, -1.0f);

			return (GetRotation() * newForward);
		}
	}
}