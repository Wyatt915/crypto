#pragma once

#include <string>
#include <vector>
#include <list>

struct CharPair;

namespace process{
	bool is_partial(std::string s);
	std::string generate_key(std::vector<CharPair> map);
	std::string merge_keys(std::list<std::string> l);
	std::string merge_keys(std::string key1, std::string key2);
	std::string merge_keys(std::vector<std::string> keys);
    std::string expand(std::string plaintext);  //adds spaces between words if they have been removed.
	std::vector<CharPair> make_charPair_vec(std::string x, std::string mapsto);
	std::vector<std::string> fill_blanks(std::string ciphertext, std::string plaintext, std::string key);
	std::vector<std::string> match_partial_words(std::string part);
}
