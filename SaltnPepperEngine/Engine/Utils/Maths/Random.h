#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{

	namespace Math
	{
		class Random32
		{
		private:

			std::mt19937 m_randomGenerator;

			//uint64_t m_count;


		public:

			static uint32_t RandomSeed();
			static Random32 Range;


			inline Random32()
				//:m_count(0)
			{

			}

			inline Random32(uint32_t seed)
				: m_randomGenerator(seed)
				//, m_count(0)
			{

			}


			inline float operator()(float min, float max)
			{
				//++m_count;
				std::uniform_real_distribution<float> realDist(min, max);
				return realDist(m_randomGenerator);
			}

			inline int32_t operator()(int32_t min, int32_t max)
			{
				//++m_count;
				std::uniform_int_distribution<int32_t> intDist(min, max);
				return intDist(m_randomGenerator);
			}

			inline uint32_t operator()(uint32_t min, uint32_t max)
			{
				//++m_count;
				std::uniform_int_distribution<uint32_t> uintDist(min, max);
				return uintDist(m_randomGenerator);
			}



			inline uint64_t GetCount() const
			{
				//return m_count;
				return 0;
			}



			// Random gen for float value
			inline float GetRandom(float min, float max)
			{
				//++m_count;
				std::uniform_real_distribution<float> realDist(min, max);
				return realDist(m_randomGenerator);
			}

			// random gen for int 32 values
			inline int32_t GetRandom(int32_t min, int32_t max)
			{
				//++m_count;
				std::uniform_int_distribution<int32_t> intDist(min, max);
				return intDist(m_randomGenerator);
			}

			// random gen for unsigned int 32 values
			inline uint32_t GetRandom(uint32_t min, uint32_t max)
			{
				//++m_count;
				std::uniform_int_distribution<uint32_t> uintDist(min, max);
				return uintDist(m_randomGenerator);
			}

			inline bool GetRandomBool()
			{
				return (bool)GetRandom(0, 1);
			}

			inline float GetRandomRotationRadians()
			{
				return GetRandom(-PI, PI);
			}

			inline float GetRandomRotationDegrees()
			{
				return GetRandom(-180.0f,180.0f);
			}

			

			// returns a random UnitVector
			inline Vector3 GetRandomUnitVector3()
			{
				// http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution
				Vector3 result;

				float phi = GetRandomRotationRadians();
				float costheta = GetRandom(-1.0f, 1.0f);
				float theta = std::acos(costheta);

				result.x = std::sin(theta) * std::cos(phi);
				result.y = std::sin(theta) * std::sin(phi);
				result.z = std::cos(theta);

				return result;

			}

			inline Vector2 GetRandomUnitVector2()
			{
				Vector2 result;
				float phi = GetRandomRotationRadians();
				result.x = std::cos(phi);
				result.y = std::sin(phi);

				return result;
			}

		};


		class Random64
		{
		private:

			std::mt19937_64 m_randomGenerator;

			//uint64_t m_count;


		public:

			static uint64_t RandomSeed();
			static Random64 Range;


			inline Random64()
				//:m_count(0)
			{

			}

			inline Random64(uint64_t seed)
				: m_randomGenerator(seed)
				//, m_count(0)
			{

			}


			inline uint64_t GetCount() const
			{
				//return m_count;
				return 0;
			}



			// Random gen for float value
			inline double GetRandom(double min, double max)
			{
				//++m_count;
				std::uniform_real_distribution<double> realDist(min, max);
				return realDist(m_randomGenerator);
			}

			// random gen for int 32 values
			inline int64_t GetRandom(int64_t min, int64_t max)
			{
				//++m_count;
				std::uniform_int_distribution<int64_t> intDist(min, max);
				return intDist(m_randomGenerator);
			}

			// random gen for unsigned int 32 values
			inline uint64_t GetRandom(uint64_t min, uint64_t max)
			{
				//++m_count;
				std::uniform_int_distribution<uint64_t> uintDist(min, max);
				return uintDist(m_randomGenerator);
			}





		};


		
	}

	using namespace Math;
}

#endif
