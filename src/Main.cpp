#include "wordlist.h"
#include "crypto_utils.h"
#include "anagram.h"
#include "analyze.h"
#include "substitution.h"
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
void test(std::string in){
	std::string key = null_key();
	std::cout << key << std::endl;
	shuffle(key);
	std::cout << key << std::endl;
	std::cout << in << '\n';
	encode(in, key);
	std::cout << in << '\n';
	strvec keys = solve_by_pattern(in);
	std::cout << keys.size();
	strvec messages;
	std::string temp;
	for (size_t i = 0; i < keys.size(); i++){
		temp = decode(in, keys[i], true);
		messages.push_back(temp);
	}
	print_list(messages, "\n\n");
}

void permutation_test(){
	std::string key = "*********PASDFGHJKLZXCVBNM";
	std::vector<std::string> keys;

	keys = fill_blanks(key);

	/*for (std::string s : keys){
		std::cout << s << '\n';
	}*/
}

int main(int argc, char* argv[]){
	load_word_list();
	std::string s = "In any operating system worthy of that name, including Windows, pointers don't designate locations on the memory chip directly. They are locations in process-specific virtual memory space and the operating system then allocates parts of the physical memory chip to store the content of the parts where the process actually stores anything on demand";
	sanitize(s);

	int c;

	while ( (c = getopt(argc, argv, "ht")) != -1 ) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
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

	return 0;
}
