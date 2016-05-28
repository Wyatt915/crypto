#pragma once
#include <string>
#include <vector>

extern std::vector<std::string> wordlist;
extern std::vector<std::string> patt;
extern std::vector<int> freqs;
extern int LENGTH_OF_LIST;
void init_words();	//Use the OG wordlist
void init_words_freq();	//Use Mark Davies' wordlist (includes frequency data)
