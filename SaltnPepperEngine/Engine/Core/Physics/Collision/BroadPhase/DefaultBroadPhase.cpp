#include "DefaultBroadPhase.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		DefaultBroadPhase::DefaultBroadPhase(const Vector3& axis)
			: m_axis(axis)
		{

		}

		DefaultBroadPhase::~DefaultBroadPhase()
		{
		}

      


		void DefaultBroadPhase::FindCollisionPairs(RigidBody3D** bodies, uint32_t count, std::vector<CollisionPair>& collisionPairs)
		{


            if (count <= 0)
            {
                // no pairs found
                return;
            }

            for (size_t i = 0; i < count - 1; ++i)
            {
                for (size_t j = i + 1; j < count; ++j)
                {
                    RigidBody3D& firstBody = *bodies[i];
                    RigidBody3D& secondBody = *bodies[j];

                    if (!firstBody.GetCollider() || !secondBody.GetCollider())
                        continue;

                    // Skip pairs of two at objects which are stationary
                    if (firstBody.GetIsStationary() && secondBody.GetIsStationary())
                        continue;

                    // Skip pairs of two at static objects
                    if (firstBody.GetIsStatic() && secondBody.GetIsStatic())
                        continue;

                    // Skip pairs of one static and one stationary
                    if (firstBody.GetIsStationary() && secondBody.GetIsStatic())
                        continue;

                    if (firstBody.GetIsStatic() && secondBody.GetIsStationary())
                        continue;

                    CollisionPair pair;

                    if (i < j)
                    {
                        pair.firstBody = &firstBody;
                        pair.secondBody = &secondBody;
                    }
                    else
                    {
                        pair.firstBody = &secondBody;
                        pair.secondBody = &firstBody;
                    }

                    bool duplicate = false;

                    // Loop through all the current objects 
                    for (int index = 0; index < collisionPairs.size(); index++)
                    {
                        CollisionPair& TempPair = collisionPairs[index];

                        if (pair.firstBody == TempPair.firstBody && pair.secondBody == TempPair.secondBody)
                        {
                            // There is a duplicate pair
                            duplicate = true;
                        }
                    }


                    if (!duplicate)
                    { // No duplicate found
                        collisionPairs.push_back(pair);
                    }
                }


            }
		}

		void DefaultBroadPhase::DebugDraw()
        {
        }
	}
}