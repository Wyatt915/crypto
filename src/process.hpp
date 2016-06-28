#pragma once

#include <string>
#include <vector>
#include <list>

struct ChaPair;

namespace process{
	bool is_partial(std::string s);
	std::string generate_key(std::vector<ChaPair> map);
	std::string merge_keys(std::list<std::string> l);
	std::string merge_keys(std::string key1, std::string key2);
	std::string merge_keys(std::vector<std::string> keys);
	std::vector<ChaPair> make_chapair_vec(std::string x, std::string mapsto);
	std::vector<std::string> fill_blanks(std::string ciphertext, std::string plaintext, std::string key);
	void match_partial_words(std::string part);
}
