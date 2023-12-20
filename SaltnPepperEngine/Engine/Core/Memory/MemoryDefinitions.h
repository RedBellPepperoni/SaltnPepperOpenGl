#ifndef MEMORYDEFINITIONS_H 
#define MEMORYDEFINITIONS_H 
#include <memory>

namespace SaltnPepperEngine
{

	namespace Memory
	{

		// Renaming Smart pointers for easier use
		template<typename T>
		using UniquePtr = std::unique_ptr<T>;

		template<typename T>
		using SharedPtr = std::shared_ptr<T>;

		template<typename T>
		using WeakPointer = std::weak_ptr<T>;

		// Wrapper around std smart unique pointer
		template<typename T, typename... Args>
		inline UniquePtr<T> MakeUnique(Args&&... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		// Wrapper around std smart shared pointer
		template<typename T, typename... Args>
		inline SharedPtr<T> MakeShared(Args&&... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		


	}


	using namespace Memory;

}

#endif

