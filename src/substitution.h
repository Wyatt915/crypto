#pragma once
#include <string>

std::string null_key();
void invert(std::string &key);
void encode(std::string &text, const std::string &key);
void decode(std::string &text, std::string key);
std::string encode(std::string text, const std::string key, bool);
std::string decode(std::string text, std::string key, bool);
std::vector<std::string> solve_by_pattern(std::string message);
