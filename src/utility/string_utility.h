#pragma once
#include <string_view>
#include <string>
#include <vector>


namespace Q {
	std::string trim(std::string_view str, std::string_view delims = " ");
	std::string trim_front(std::string_view str, std::string_view delims);
	std::string trim_back(std::string_view str, std::string_view delims);

	std::vector<std::string> split(const std::string_view& s, char delim);

	std::vector<std::string> split_once(const std::string& s, char delim);

	std::vector<std::string> split(const std::string_view& str, const std::string_view& delims = " ");

	std::string replace_all(std::string str, std::string_view from, std::string_view to);
}