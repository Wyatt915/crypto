#pragma once
#include <array>
#include <list>
#include <numeric>
#include <stdio.h>
#include <string>
#include <vector>

namespace analyze {
    double chi_sq(std::string in);
    double idx_coin(std::string in); //index of coincidence
	bool is_complete(std::string word);
	bool is_known(std::string word);
	bool key_conflict(std::string, std::string);
	int prob_score(std::string plaintext);
	std::array<long int, 26> count_chars(std::string in); //[in] must be UPPERCASE
    std::array<double, 26> frequency(std::string in);
	std::string char_pattern(std::string in);
	std::string char_vector_to_string(std::vector<char>);
	std::string missing_key_values(std::string);
	std::vector<bool> used_unknowns(std::string ciphertext, std::string plaintext);
	void print_histogram(std::string in, int resolution);
} /* analyze */
