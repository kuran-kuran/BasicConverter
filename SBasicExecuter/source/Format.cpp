#include <stdarg.h>
#include <vector>
#include "String.hpp"

namespace dms
{
	dms::String Format(const char* format, ...)
	{
		int text_buffer_size = 128;
		std::vector<char> text_buffer(static_cast<size_t>(text_buffer_size) + 1);
		va_list arglist;
		va_start(arglist, format);
		int vssize = vsnprintf(&text_buffer[0], text_buffer_size, format, arglist);
		va_end(arglist);
		if((vssize >= 0) && (vssize < text_buffer_size))
		{
			text_buffer.resize(vssize);
			return dms::String(&text_buffer[0], &text_buffer[text_buffer.size() - 1]);
		}
#ifdef _WIN32
		va_start(arglist, format);
		vssize = _vscprintf(format, arglist);
		va_end(arglist);
#endif
		if(vssize < 0)
		{
			return "";
		}
		text_buffer.resize(static_cast<size_t>(vssize) + 1);
		va_start(arglist, format);
		vssize = vsnprintf(&text_buffer[0], static_cast<size_t>(vssize) + 1, format, arglist);
		va_end(arglist);
		if(vssize < 0)
		{
			return "";
		}
		text_buffer.resize(vssize);
		return dms::String(&text_buffer[0], &text_buffer[text_buffer.size() - 1]);
	}
};
