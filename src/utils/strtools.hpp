#pragma once
#include <string>
#include <sstream>
#include <vector>

namespace strtools
{
	template <int BufSize = 1024, typename ...Args>
	std::string catf(const char* format, Args...args)
	{
		static char buf[BufSize];
		sprintf_s(buf, format, args...);
		return std::string(buf);
	}

	template <typename T>
	std::string cat(T t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template <typename Out>
	void split(const std::string& str, char delim, Out result)
	{
		std::istringstream iss(str);
		std::string item;
		while (std::getline(iss, item, delim)) {
			*result++ = item;
		}
	}

	std::vector<std::string> split(const std::string& str, char delim);
}

