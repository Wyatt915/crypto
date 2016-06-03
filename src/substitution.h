#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<std::string>>	str_2D;
typedef std::vector<std::string>				strvec;

std::string null_key();
std::string empty_key();
void optimize(str_2D&);
strvec solve_by_pattern(std::string&)
