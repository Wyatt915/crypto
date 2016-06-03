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

typedef std::vector<std::vector<std::string>>	str_2D;
typedef std::vector<std::string>				strvec;

const int ALPHA = 26;
str_2D sorted;
str_2D psorted;

struct IntPair{
	int x, y;
};

void invert(std::string& key){
	std::string inv(ALPHA, '*');
	for (int i = 0; i < ALPHA; i++){
		if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
	}
	key = inv;
}

void encode(std::string& text, const std::string& key){
	capitalize(text);
	std::string ciphertext;

	for (size_t i = 0; i < text.length(); i++){
		if (!(text[i] >= 'A' && text[i] <= 'Z')){
			ciphertext += text[i]; //ignore non-alpha chars
		}
		else{ text[i] = key[text[i] - 'A']; }
	}
}

void decode(std::string& text, std::string key){
	invert(key);
	encode(text, key);
}

std::string encode(std::string text, const std::string key, bool){
	capitalize(text);
	std::string ciphertext;

	for (size_t i = 0; i < text.length(); i++){
		if (!(text[i] >= 'A' && text[i] <= 'Z')){
			ciphertext += text[i]; //ignore non-alpha chars
		}
		else{ text[i] = key[text[i] - 'A']; }
	}
	return text;
}

std::string decode(std::string text, std::string key, bool){
	invert(key);
	std::string s = encode(text, key, true);
	return s;
}

//returns a list of all words of the same pattern
strvec match_by_pattern(std::string in){
	strvec out;
	std::string pattern = char_pattern(in);
	for (int i = 0; i < LENGTH_OF_LIST; i++)
	{
		if (pattern == patt[i]){ out.push_back(wordlist[i]); }
	}
	return out;
}

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
	remove_dp(in);	//sanitize
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
	std::string s = "In any operating system worthy of that name, including Windows,
		pointers don't designate locations on the memory chip directly. They are
		locations in process-specific virtual memory space and the operating system then
		allocates parts of the physical memory chip to store the content of the parts
		where the process actually stores anything on demand";
	remove_dp(s);
	test(s);
	char c;
	std::cin >> c;
	return 0;
}
