#include "wordlist.h"
#include "crypto_utils.h"
#include "analyze.h"
#include "substitution.h"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <getopt.h>

typedef std::vector<std::vector<std::string>>	str_2D;
typedef std::vector<std::string>				strvec;

const int ALPHA = 26;
str_2D sorted;
str_2D psorted;

struct IntPair{
	int x, y;
};

strvec empatternate(strvec& input){
	strvec pattern_list;
	pattern_list.resize(LENGTH_OF_LIST);
	for (int i = 0; i < LENGTH_OF_LIST; i++){
		pattern_list[i] = char_pattern(input[i]);
	}
	return pattern_list;
}

void load_word_list(){
	init_words();
	patt = empatternate(wordlist);
}

void autosolve(std::string ciphertext){
	strvec incompleteKeys = solve_by_pattern(ciphertext);
	std::cout << incompleteKeys.size() << " Unique keys found.\n\n" << std::endl;

	strvec messages;
	std::string temp;
	for (size_t i = 0; i < incompleteKeys.size(); i++){
		invert(incompleteKeys[i]);	//VERY IMPORTANT (to maintain proper mapping in the fill_blanks function)
		temp = encode(ciphertext, incompleteKeys[i], true);
		messages.push_back(temp);
		std::cout << temp << std::endl;
	}

	strvec keys, tempkeys;
	for (size_t i = 0; i < incompleteKeys.size(); i++){
		tempkeys = fill_blanks(ciphertext, messages[i], incompleteKeys[i]);
		keys.insert(keys.end(), tempkeys.begin(), tempkeys.end());
	}

	messages.clear();

	std::cout << "Only " << keys.size() << " permutations to go!" << std::endl;

	std::vector<int> rank;
	int r = 0;
	for (size_t i = 0; i < keys.size(); i++) {
		temp = encode(ciphertext, keys[i], true);
		rank.push_back(prob_score(temp));
		messages.push_back(temp);
	}

	std::vector<int>::iterator maximum = std::max_element(rank.begin(), rank.end());
	std::vector<int>::iterator minimum = std::min_element(rank.begin(), rank.end());

	int idx = std::distance(rank.begin(), maximum);
	std::cout << ciphertext << "\n\n";
	std::cout << messages[idx] << "\n\n";
	std::cout << "Max: " << *maximum << "\tMin: " << *minimum;

}

void test(std::string in){
	std::string key = null_key();
	std::cout << key << std::endl;
	shuffle(key);
	std::cout << key << std::endl;
	std::cout << in << "\n\n";
	encode(in, key);
	std::cout << in << "\n\n";
	autosolve(in);
}

int main(int argc, char* argv[]){
	load_word_list();
	std::string s = "In any operating system worthy of that name, including Windows, pointers don't designate locations on the memory chip directly. They are locations in process-specific virtual memory space and the operating system then allocates parts of the physical memory chip to store the content of the parts where the process actually stores anything on demand";
	sanitize(s);

	int c;
	char *avalue = NULL;
	char *evalue = NULL;
	std::string avalue_str;
	std::string evalue_str;
	bool a = false;
	while ( (c = getopt(argc, argv, "a:e:ht")) != -1 ) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
			case 'a':
				avalue = optarg;
				avalue_str = std::string(avalue);
				sanitize(avalue_str);
				a = true;
				break;
			case 'e':
				evalue = optarg;
				evalue_str = std::string(evalue);
				break;
			case 'h':
				std::cout << "Help message." << std::endl;
				break;
			case 't':
				test(s);
				break;
			default:
				std::cout << "Bruh." << std::endl;
				break;
		}
	}
	if(a){
		autosolve(avalue_str);
	}
	return 0;
}
