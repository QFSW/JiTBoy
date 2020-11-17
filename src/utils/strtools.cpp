#include "strtools.hpp"

namespace strtools
{
	std::vector<std::string> split(const std::string& str, const char delim)
	{
		std::vector<std::string> elems;
		split(str, delim, std::back_inserter(elems));
		return elems;
	}

	std::string colorize(const std::string& str, const AnsiColor col)
	{
		return catf("\033[1;%dm%s\033[0m", col, str.c_str());
	}
}