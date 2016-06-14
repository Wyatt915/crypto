#include "wordlist.h"
#include "superlist.h"
#include <sstream>

std::vector<std::string> temp;
std::vector<int> ftemp;


std::vector<std::string> wordlist = temp;	//"definition"
std::vector<std::string> patt = temp;
std::vector<int> freqs = ftemp;
int LENGTH_OF_LIST = 0;

void init_words(){
	std::string raw(words, words_len);
	std::stringstream str(raw);
	std::string current = "";
	int f;
	while (str >> current >> f){
		wordlist.push_back(current);
		freqs.push_back(f);
		LENGTH_OF_LIST++;
	}
}
