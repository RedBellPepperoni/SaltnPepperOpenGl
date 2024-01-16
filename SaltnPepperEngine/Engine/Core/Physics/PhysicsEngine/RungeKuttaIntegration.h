#ifndef RUNGEKUTTAINTEGRATION_H
#define RUNGEKUTTAINTEGRATION_H



#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{

	namespace Physics
	{
		class RungeKuttaIntegration
		{

		public:

			struct State
			{
				Vector3 position = Vector3(0.0f);
				Vector3 velocity = Vector3(0.0f);
				Vector3 acceleration = Vector3(0.0f);
			};

			struct Derivative
			{
				Vector3 acceleration = Vector3(0.0f);
				Vector3 velocity = Vector3(0.0f);
			};

		public:

			static void RungeKutta2(State& state, float t, float dt);
			static void RungeKutta4(State& state, float t, float dt);

			static Derivative Calculate(State& initial, float dt, float t, const Derivative& derivative);

		};
	}

}

#endif // !RUNGEKUTTAINTEGRATION_H

