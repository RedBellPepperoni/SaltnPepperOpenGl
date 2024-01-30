#pragma once
#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		class RigidBody3D;

#define MAX_CONTACTS 4

		struct ContactPoint
		{
			float totalImpulsefromContact = 0.0f;
			float totalImpulsefromFriction = 0.0f;

			float collisionPenetratin = 0.0f;

			float elasticityTerm = 0.0f;

			Vector3 collisionNormal;
			Vector3 relativePosOne;
			Vector3 relativePosTwo;


		};	


		class Manifold
		{
		public:

			Manifold();
			~Manifold();

			// For each collision pair
			void Initilize(RigidBody3D* bodyOne, RigidBody3D* bodyTwo);

			void AddContactPoint(const Vector3& pointOnOne, const Vector3& pointOnTwo, const Vector3& contactNormal, const float& penetration);

			void ApplyImpulse();
			void SolveElasticity(float deltaTime);


			RigidBody3D* GetBodyOne() const;
			RigidBody3D* GetBodyTwo() const;
				

		protected:

			void SolveContactPoint(ContactPoint& point) const;

			void SolveContraints(ContactPoint& point);


		protected:

			RigidBody3D* m_bodyOne;
			RigidBody3D* m_bodyTwo;

			ContactPoint m_contactPoints[MAX_CONTACTS];

			uint32_t m_contactCount = 0;
		};
	}
}

