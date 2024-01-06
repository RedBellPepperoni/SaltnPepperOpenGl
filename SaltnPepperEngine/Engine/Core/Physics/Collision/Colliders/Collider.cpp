#include "Collider.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{


		Collider::Collider()
			: m_type(ColliderType::NONE)
			, m_transform(Matrix4(1.0f))
		{
		}

		Collider::~Collider()
		{
			
		}

		void Collider::SetTransform(const Matrix4& transform)	
		{
			m_transform = transform;
		}

		ColliderType Collider::GetType() const
		{
			return m_type;
		}

		

	}
}