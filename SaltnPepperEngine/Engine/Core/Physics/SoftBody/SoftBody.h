#ifndef SOFTBODY_H
#define SOFTBODY_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include <vector>

namespace SaltnPepperEngine
{
	namespace Physics
	{

		namespace SoftBodyMath
		{
			
		}


		class SoftBody
		{

		private:

			// Total number of particles to Spawn for this Mesh (Equal to the number of Vertices)
			size_t numParticles = 0;
			// The actual number of tetrahedral faces
			size_t numTets = 0;

			
			std::vector<Vector3> vertPositions = std::vector<Vector3>();
			std::vector<Vector3> vertPreviousPositions = std::vector<Vector3>();

			std::vector<Vector3> velocities = std::vector<Vector3>();




		};


	}
}


#endif // !SOFTBODY_H

