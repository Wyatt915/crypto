#pragma once
#include <string>
#include <vector>

extern std::vector<std::string> wordlist;
extern std::vector<std::string> patt;
extern std::vector<int> freqs;
extern int LENGTH_OF_LIST;
extern const int AVERAGE_FREQ;
extern const int MEDIAN_FREQ;

void init_words();
