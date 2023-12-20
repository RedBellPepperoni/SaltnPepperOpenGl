#include "Random.h"


namespace SaltnPepperEngine
{

	namespace Math
	{

		Random32 Random32::Range = Random32(Random32::RandomSeed());
		Random64 Random64::Range = Random64(Random64::RandomSeed());


		uint32_t Random32::RandomSeed()
		{
			std::random_device device;
			return device();
		}

		uint64_t Random64::RandomSeed()
		{
			// since device gives only 32 bit value
			std::random_device device;

			// store it in a 64bit register
			uint64_t value = device();

			// generatet the remaining bits
			return (value << 32) | device();
		}

	}

}
