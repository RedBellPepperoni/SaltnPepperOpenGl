#include "RungeKuttaIntegration.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void RungeKuttaIntegration::RungeKutta2(State& state, float t, float dt)
		{
			const Derivative a = Calculate(state, t, 0.0f, Derivative());
			const Derivative b = Calculate(state, t, dt * 0.5f, a);

			const Vector3 dxdt = (a.velocity + b.velocity) * 0.5f;
			const Vector3 dvdt = (a.acceleration + b.acceleration) * 0.5f;

			state.position += dxdt * dt;
			state.velocity += dvdt * dt;
		}

		void RungeKuttaIntegration::RungeKutta4(State& state, float t, float dt)
		{
			const Derivative a = Calculate(state, t, 0.0f, Derivative());
			const Derivative b = Calculate(state, t, dt * 0.5f, a);
			const Derivative c = Calculate(state, t, dt * 0.5f, b);
			const Derivative d = Calculate(state, t, dt, c);

			const Vector3 dxdt = (a.velocity + (b.velocity + c.velocity) * 2.0f + d.velocity) * 1.0f / 6.0f;
			const Vector3 dvdt = (a.acceleration + (b.acceleration + c.acceleration) * 2.0f + d.acceleration) * 1.0f / 6.0f;

			state.position += dxdt * dt;
			state.velocity += dvdt * dt;
		}

		RungeKuttaIntegration::Derivative RungeKuttaIntegration::Calculate(State& initial, float dt, float t, const Derivative& derivative)
		{
			State state;
			state.position = initial.position + derivative.velocity * dt;
			state.velocity = initial.velocity + derivative.acceleration * dt;

			Derivative output;
			output.velocity = state.velocity;
			output.acceleration = initial.acceleration;

			return output;
		}
	}


}