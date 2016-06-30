#include "crypto_utils.hpp"
#include "superlist.inc"
#include "wordlist.hpp"

#include <sstream>

utils::VString temp;
std::vector<int> ftemp;


utils::VString WORDLIST = temp;	//"definition"
utils::VString PATTLIST = temp;
std::vector<int> FREQLIST = ftemp;
int LENGTH_OF_LIST = 0;

const int AVERAGE_FREQ = 50;	//Really it's closer to 49.9102541882083
const int MEDIAN_FREQ  = 34;	//It's exactly 34

void init_words(){
	std::string raw(superlist, superlist_len);
	std::stringstream str(raw);
	std::string current = "";
	int f;
	while (str >> current >> f){
		WORDLIST.push_back(current);
		FREQLIST.push_back(f);
		LENGTH_OF_LIST++;
	}
}
