#include "wordlist.h"
#include <sstream>
std::vector<std::string> temp;
std::vector<std::string> wordlist = temp;	//"definition"
std::vector<std::string> patt = temp;

void init_words(){
	std::string raw(WORDLIST, wordlist_len);
	std::stringstream str(raw);
	std::string current = "";
	while (str >> current){
		wordlist.push_back(current);
	}
}