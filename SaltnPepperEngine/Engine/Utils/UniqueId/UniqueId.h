#ifndef UNIQUEID_H
#define UNIQUEID_H
#include <stdint.h>


namespace SaltnPepperEngine
{
	class UniqueId
	{
	private:

		uint64_t m_uniqueId;

	public:

		UniqueId();
		UniqueId(uint64_t uniqueId);
		UniqueId(const UniqueId& otherid);

		~UniqueId() = default;

		const uint64_t GetId() const
		{
			return m_uniqueId;
		}



	};
		
}



#endif // !UNIQUEID_H




