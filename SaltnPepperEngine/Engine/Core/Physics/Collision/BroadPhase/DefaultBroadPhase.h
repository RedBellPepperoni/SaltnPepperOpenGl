#pragma once
#include "BroadPhase.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>


namespace SaltnPepperEngine
{
	namespace Physics
	{

		// A very basic algorithm to generate collision pairs for broadPhase
		// Basically brute forcing through all the Bodies

		class DefaultBroadPhase : public BroadPhase
		{
		public:

			explicit DefaultBroadPhase(const Vector3& axis = Vector3(0.0f));

			virtual ~DefaultBroadPhase();


			void FindCollisionPairs(SharedPtr<RigidBody3D>* bodies, uint32_t count, std::vector<CollisionPair>& collisionPairs) override;

			void DebugDraw() override;

		protected:

			Vector3 m_axis;

		};

	}
}

