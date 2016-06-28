#include "superlist.inc"
#include "wordlist.h"

#include <sstream>

std::vector<std::string> temp;
std::vector<int> ftemp;


std::vector<std::string> wordlist = temp;	//"definition"
std::vector<std::string> patt = temp;
std::vector<int> freqs = ftemp;
int LENGTH_OF_LIST = 0;

const int AVERAGE_FREQ = 50;	//Really it's closer to 49.9102541882083
const int MEDIAN_FREQ  = 34;	//It's exactly 34

void init_words(){
	std::string raw(superlist, superlist_len);
	std::stringstream str(raw);
	std::string current = "";
	int f;
	while (str >> current >> f){
		wordlist.push_back(current);
		freqs.push_back(f);
		LENGTH_OF_LIST++;
	}
}
