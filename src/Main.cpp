#include "wordlist.h"
#include "crypto_utils.h"
#include "analyze.h"
#include "substitution.h"
#include "meta.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
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

void test(){
	std::string s = "In any operating system worthy of that name, including "
	"Windows, pointers don't designate locations on the memory chip directly. "
	"They are locations in process-specific virtual memory space and the "
	"operating system then allocates parts of the physical memory chip to store "
	"the content of the parts where the process actually stores anything on demand";
	sanitize(s);
	std::string key = null_key();
	std::cout << key << std::endl;
	shuffle(key);
	std::cout << key << std::endl;
	std::cout << s << "\n\n";
	encode(s, key);
	std::cout << s << "\n\n";
	autosolve(s);
}

std::string read(std::string filename){
	std::string line;
	std::stringstream data;
	std::ifstream infile(filename.c_str());
	if(infile.is_open()){
		while(std::getline(infile,line)){
			data << line;
		}
		infile.close();
	}
	return data.str();
}

void write(std::string filename, std::string data){
	std::ofstream outfile (filename, std::ios::out);
	if(outfile.is_open()) {
		outfile << data;
		outfile.close();
	}
}

int main(int argc, char* argv[]){
	load_word_list();

	int c;

	std::string avalue_str;
	std::string dvalue_str;
	std::string evalue_str;
	std::string kvalue_str;
	std::string ovalue_str;
	bool a_opt = false;
	bool d_opt = false;
	bool e_opt = false;
	bool f_opt = false;
	bool k_opt = false;
	bool o_opt = false;
	while ( (c = getopt(argc, argv, "a:d:e:k:o:fht")) != -1 ) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
			case 'a':
				avalue_str = std::string(optarg);
				sanitize(avalue_str);
				a_opt = true;
				break;
			case 'd':
				d_opt = true;
				dvalue_str = std::string(optarg);
				break;
			case 'e':
				e_opt = true;
				evalue_str = std::string(optarg);
				break;
			case 'f':
				f_opt = true;
				break;
			case 'h':
				std::cout << help << std::endl;
				return 0;
			case 'k':
				k_opt = true;
				kvalue_str = std::string(optarg);
				break;
			case 'o':
				o_opt = true;
				ovalue_str = std::string(optarg);
				break;
			case 't':
				test();
				break;
			default:
				std::cout << "Bruh." << std::endl;
				break;
		}
	}
	if(a_opt){
		if(f_opt){ avalue_str = read(avalue_str); }
		sanitize(avalue_str);
		autosolve(avalue_str);
		return 0;
	}
	if(e_opt){
		if(f_opt){ evalue_str = read(evalue_str); }
		if(!k_opt){
			kvalue_str = null_key();
			shuffle(kvalue_str);
			std::cout << "Using randomly generated key: " << kvalue_str << std::endl;
		}
		sanitize(evalue_str);
		encode(evalue_str, kvalue_str);
		if(!o_opt){ std::cout << evalue_str << std::endl; }
		else{ write(ovalue_str, evalue_str); }
	}
	if(d_opt && k_opt){
		if(f_opt){ dvalue_str = read(dvalue_str); }
		sanitize(dvalue_str);
		decode(dvalue_str, kvalue_str);
		std::cout << dvalue_str << std::endl;
	}
	return 0;
}
