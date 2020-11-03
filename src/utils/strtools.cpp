#include "strtools.hpp"

namespace strtools
{
	std::vector<std::string> split(const std::string& str, char delim)
	{
		std::vector<std::string> elems;
		split(str, delim, std::back_inserter(elems));
		return elems;
	}
}