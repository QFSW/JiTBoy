#pragma once
#include <string>

namespace strtools
{
	template <int BufSize = 1024, typename ...Args>
	std::string catf(const char* format, Args ...args)
	{
		static char buf[BufSize];
		sprintf_s(buf, format, args...);
		return std::string(buf);
	}
}

