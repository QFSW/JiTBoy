#pragma once
#include <string>
#include <sstream>
#include <vector>

#include <utils/traits.hpp>

namespace strtools
{
	template <int BufSize = 1024, typename ...Args>
	std::string catf(const char* format, Args...args)
	{
		static_assert(!traits::for_any<std::is_class, Args...>(), "strtools::catf does not work with classes");

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

	enum class AnsiColor
	{
	    Black = 30,
		Blue = 34,
		Green = 32,
		Cyan = 36,
		Red = 31,
		Purple = 35,
		Brown = 33,
		Gray = 37
	};

	std::string colorize(const std::string& str, AnsiColor col);
}

