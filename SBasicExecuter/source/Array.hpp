#include "Variable.hpp"

namespace dms
{
	template < typename T, std::size_t N >
	struct Array
	{
		T storage[N];
		using reference = T&;
		using const_reference = T const &;

		reference operator [] (std::size_t i)
		{
			return storage[i];
		}

		const_reference operator [] (std::size_t i) const
		{
			return storage[i];
		}

		reference operator [] (dms::Variable i)
		{
			return storage[i.GetInt()];
		}

		const_reference operator [] (dms::Variable i) const
		{
			return storage[i.GetInt()];
		}

		void clear(void)
		{
			size_t storageSize = sizeof(storage) / sizeof(T);
			for(size_t i = 0; i < storageSize; ++ i)
			{
				storage[i].clear();
			}
		}
	};
}
