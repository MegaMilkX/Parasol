#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> &split(const std::string& s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string remove_all_of(const std::string& s, char c)
{
	std::string str = s;
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
	return str;
}

#endif