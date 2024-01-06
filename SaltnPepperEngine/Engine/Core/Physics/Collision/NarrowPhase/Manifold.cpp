#include "Manifold.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Physics/PhysicsEngine/PhysicsEngine.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{


	

		Manifold::Manifold()
			: m_bodyOne(nullptr)
			, m_bodyTwo(nullptr)
		{
		}

		Manifold::~Manifold()
		{
		}

		void Manifold::Initilize(RigidBody3D* bodyOne, RigidBody3D* bodyTwo)
		{
			m_contactCount = 0;

			m_bodyOne = bodyOne;
			m_bodyTwo = bodyTwo;
		}

		void Manifold::AddContactPoint(const Vector3& pointOnOne, const Vector3& pointOnTwo, const Vector3& contactNormal, const float& penetration)
		{
			Vector3 relativeOne = (pointOnOne - m_bodyOne->GetPosition());
			Vector3 relativeTwo = (pointOnTwo - m_bodyTwo->GetPosition());

			ContactPoint newpoint;

			newpoint.relativePosOne = relativeOne;
			newpoint.relativePosTwo = relativeTwo;
			newpoint.collisionNormal = contactNormal;
			newpoint.collisionPenetratin = penetration;
			newpoint.totalImpulsefromContact = 0.0f;
			newpoint.totalImpulsefromFriction = 0.0f;

			const float minAllowedDistanceSquared = Square(0.2f);
			bool shouldAdd = true;


			// Check if we have multiple poiunts in the same posiiotn (almost same)
			for (uint32_t index = 0; index < m_contactCount; index++)
			{
				Vector3 vectorOneTwo = m_contactPoints[index].relativePosOne - newpoint.relativePosOne;

				// get the squared magnitude of the vector
				float distanceSquared = Dot(vectorOneTwo, vectorOneTwo);
				
				// Get the point with the latgest penetreaion
				if (distanceSquared < minAllowedDistanceSquared)
				{
					if (m_contactPoints[index].collisionPenetratin > newpoint.collisionPenetratin)
					{
						std::swap(m_contactPoints[index], m_contactPoints[m_contactCount - 1]);
						m_contactCount--;
						continue;
					}
					else
					{
						shouldAdd = false;
					}
				}

			}

			if (shouldAdd)
			{
				m_contactPoints[m_contactCount] = newpoint;
				m_contactCount++;
			}

		}

		void Manifold::ApplyImpulse()
		{

			for (uint32_t index = 0; index < m_contactCount; index++)
			{
				SolveContactPoint(m_contactPoints[index]);
			}
		}


		void Manifold::SolveElasticity(float deltaTime)
		{
			for (uint32_t i = 0; i < m_contactCount; i++)
			{
				SolveContraints(m_contactPoints[i]);
			}
		}

	

		RigidBody3D* Manifold::GetBodyOne() const
		{
			return m_bodyOne;
		}

		RigidBody3D* Manifold::GetBodyTwo() const
		{
			return m_bodyTwo;
		}

		void Manifold::SolveContactPoint(ContactPoint& point) const
		{
			if (m_bodyOne->m_invMass + m_bodyTwo->m_invMass == 0.0f)
			{
				return;
			}

			Vector3& relativeOne = point.relativePosOne;
			Vector3& relativeTwo = point.relativePosTwo;


			Vector3 velocityOne = m_bodyOne->m_velocity;// Add angular studff here later if possible
			Vector3 velocityTwo = m_bodyTwo->m_velocity;

			Vector3& normal = point.collisionNormal;
			Vector3 velocityDirection = velocityOne - velocityTwo;



			// Contact Collision Check 

			const float Mass = (m_bodyOne->m_invMass + m_bodyTwo->m_invMass) + Dot(normal,Cross(m_bodyOne->m_inverseInertia* Cross(relativeOne, normal),relativeOne)+ Cross(m_bodyTwo->m_inverseInertia * Cross(relativeTwo,normal),relativeTwo));
			

			//  "Baumgarte Stabilization"

			 // Amount of force to add to the System to solve error
			const float baumgarteScalar = 0.3f;  

			// Amount of allowed penetration, ensures a complete manifold each frame
			const float baumgarteSlop = 0.001f; 


			float penetrationSlop = fmin(point.collisionPenetratin + baumgarteSlop, 0.0f);
			float b = -(baumgarteScalar / PhysicsEngine::GetDeltaTime()) * penetrationSlop;
			float b_real = fmax(b, point.elasticityTerm + b * 0.2f);
			float jn = -(Dot(velocityDirection, normal) + b_real) / Mass;
			float oldSumImpulseContact = point.totalImpulsefromContact;

			jn = fmin(jn, 0.0f);
			point.totalImpulsefromContact = fmin(point.totalImpulsefromContact + jn, 0.0f);
			jn = point.totalImpulsefromContact - oldSumImpulseContact;

			m_bodyOne->SetVelocity((velocityOne + normal * (jn *m_bodyOne->m_invMass)) );
			m_bodyTwo->SetVelocity((velocityTwo - normal * (jn * m_bodyTwo->m_invMass)));



			 //Friction
			//{
			//	Vector3 tangent = velocityDirection - normal * Dot(velocityDirection, normal);
			//	float lengthTangent = Length(tangent);

			//	if (lengthTangent > 0.0001f)
			//	{
			//		tangent = tangent * (1.0f / lengthTangent);

			//		float frictionalMass = (m_bodyOne->m_invMass + m_bodyTwo->m_invMass)
			//			+ Dot(tangent, Cross(m_bodyOne->m_inverseInertia * Cross(relativeOne, tangent), relativeOne) + Cross(m_bodyTwo->m_inverseInertia * Cross(relativeTwo, tangent), relativeTwo));

			//		float fricOne = fmax(m_bodyOne->GetFriction(), 0.1f);
			//		float fricTwo = fmax(m_bodyTwo->GetFriction(), 0.1f);

			//		float frictionCoeff = sqrtf(fricOne * fricTwo);
			//		float jt = -1.0f * frictionCoeff * Dot(velocityDirection, tangent) / frictionalMass;

			//		// Clamp so that the frictioan never applies more force than the collision

			//		float oldImpulseTangent = point.totalImpulsefromFriction;
			//		float maxJt = frictionCoeff * point.totalImpulsefromContact;
			//		point.totalImpulsefromFriction = fmin(fmax(oldImpulseTangent + jt, maxJt), -maxJt);




			//		jt = point.totalImpulsefromFriction - oldImpulseTangent;

			//		

			//		m_bodyOne->SetVelocity(m_bodyOne->GetVelocity() + tangent * (jt * m_bodyOne->m_invMass));
			//		m_bodyTwo->SetVelocity(m_bodyTwo->GetVelocity() - tangent * (jt * m_bodyTwo->m_invMass));



			//	}
			//}


		}

		void Manifold::SolveContraints(ContactPoint& point)
		{

			point.totalImpulsefromContact = 0.0f;
			point.totalImpulsefromFriction = 0.0f;


			const float elasticity = sqrtf(m_bodyOne->GetElasticity() * m_bodyTwo->GetElasticity());

			float elasticity_term = elasticity * Dot(point.collisionNormal, (m_bodyOne->GetVelocity() - m_bodyTwo->GetVelocity()));


			if (elasticity_term < 0.0f)
			{
				point.elasticityTerm = 0.0f;
			}
			else
			{
				const float elasticity_slope = 0.2f;

				if (elasticity_term < elasticity_slope)
				{
					elasticity_term = 0.0f;

				}

				point.elasticityTerm = elasticity_term;
			}


		}

		
	}
}
