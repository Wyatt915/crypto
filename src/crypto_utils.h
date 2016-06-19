#pragma once
#include <string>
#include <vector>



void capitalize(std::string&);
std::string char_vector_to_string(std::vector<char>);
void print_list(const std::vector<std::string>&);
void print_list(const std::vector<std::string>&, char);
void print_list(const std::vector<std::string>&, std::string);
void sanitize(std::string&);
void shuffle(std::string&);
void remove_p(std::string&);
void remove_dp(std::string&);
void remove_dpw(std::string&);//remove digits, punctuation, and spaces
std::vector<std::string> permute(std::string);
int binary_search(const std::vector<std::string> &v, std::string what);
