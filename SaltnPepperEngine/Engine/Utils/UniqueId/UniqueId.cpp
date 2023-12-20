#include "UniqueId.h"
#include "Engine/Utils/Maths/Random.h"

namespace SaltnPepperEngine
{
	UniqueId::UniqueId()
	{
		// Generating a random Unique ID from 64bit int.
		m_uniqueId = Random64::Range.GetRandom(0, std::numeric_limits<uint64_t>::max());
	}

	UniqueId::UniqueId(uint64_t uniqueId)
		: m_uniqueId(uniqueId)
	{

	}
	UniqueId::UniqueId(const UniqueId& otherid)
		: m_uniqueId(otherid.GetId())
	{
	}
}
