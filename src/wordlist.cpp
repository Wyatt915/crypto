#include "wordlist.h"
#include "words.h"
#include "freq.h"
#include <sstream>

std::vector<std::string> temp;
std::vector<int> ftemp;
std::vector<std::string> wordlist = temp;	//"definition"
std::vector<std::string> patt = temp;
std::vector<int> freq = ftemp;
int LENGTH_OF_LIST = 0;

void init_words(){
	std::string raw(WORDLIST, wordlist_len);
	std::stringstream str(raw);
	std::string current = "";
	while (str >> current){
		wordlist.push_back(current);
		LENGTH_OF_LIST++;
	}
}

void init_words_freq(){
	std::string raw(WORDLIST, wordlist_len);
	std::stringstream str(raw);
	std::string current = "";
	int f = 0;
	while (str >> current >> f){
		wordlist.push_back(current);
		freq.push_back(f);
		LENGTH_OF_LIST++;
	}
}