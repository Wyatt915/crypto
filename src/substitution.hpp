#pragma once
#include <string>

namespace subst{
	void fine_solve(std::string message, std::string courseKeys);
	std::string decode(std::string text, std::string key, bool);
	std::string encode(std::string text, const std::string key, bool);
	std::string invert(std::string key, bool a);
	std::string null_key();
	std::vector<std::string> solve_by_pattern(std::string message);
	void decode(std::string &text, std::string key);
	void encode(std::string &text, const std::string &key);
	void invert(std::string &key);
}
