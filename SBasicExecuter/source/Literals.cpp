#include "Executer.hpp"

// User defined literals
dms::String operator "" _s(const char* str, std::size_t length)
{
	return dms::String(str);
}

dms::Variable operator "" _n(unsigned long long x)
{
	return static_cast<int>(x);
}

dms::Variable operator "" _f( long double value )
{
	return static_cast<double>(value);
}
