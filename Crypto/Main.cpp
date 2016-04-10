#define GVARS	//for the wordlist in resource_handler.h

#include "crypto_utils.h"
#include "resource_handler.h"
#include "anagram.h"
#include "analyze.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <list>
#include <process.h>

#define TAB '\t'

using namespace std;

typedef vector<vector<string>>	str_2D;
typedef vector<string>			strvec;

const int ALPHA = 26;
str_2D sorted;
str_2D psorted;

struct ChaPair{
	char x, mapsTo;
};

struct IntPair{
	int x, y;
};

void invert(string& key){
	string inv(ALPHA, '*');
	for (int i = 0; i < ALPHA; i++){
		inv[key[i] - 'A'] = i + 'A';
	}
	key = inv;
}

void encode(string& text, const string& key){
	capitalize(text);
	string ciphertext;

	for (size_t i = 0; i < text.length(); i++){
		if (!(text[i] >= 'A' && text[i] <= 'Z')){
			ciphertext += text[i]; //ignore non-alpha chars
		}
		else{ text[i] = key[text[i] - 'A']; }
	}
}

void decode(string& text, string key){
	invert(key);
	encode(text, key);
}

str_2D sort_by_length(strvec in){
	size_t len = 0;
	for (int i = 0; i < Handler::LENGTH_OF_LIST; i++){	//get maximum word length
		if (in[i].length() > len){ len = in[i].length(); }
	}
	len++;
	int *temp = new int[len];
	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s

	for (int i = 0; i < Handler::LENGTH_OF_LIST; i++){
		temp[in[i].length()]++;	//get number of words for each length
	}

	str_2D out(len);

	for (size_t i = 0; i < len; i++){
		out[i].resize(temp[i]); //initialize 2d array in the form [length][index_of_word]
	}

	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s
	//temp is now used to keep track of #words in each row
	for (int i = 0; i < Handler::LENGTH_OF_LIST; i++){
		out[in[i].length()][temp[in[i].length()]] = in[i];
		temp[in[i].length()]++;
	}
	delete[] temp;
	return out;
}

//returns a list of all words of the same pattern
strvec match_by_pattern(string in){
	strvec out;
	string pattern = char_pattern(in);
	for (size_t i = 0; i < Handler::LENGTH_OF_LIST; i++)
	{
		if (pattern == patt[i]){ out.push_back(wordlist[i]); }
	}
	return out;
}

string generate_key(vector<ChaPair> map){
	string out = "**************************";
	for (size_t i = 0; i < map.size(); i++)
	{
		out[map[i].x - 'A'] = map[i].mapsTo;
	}
	return out;
}

//x and mapsto will always be the same length
vector<ChaPair> make_chapair_vec(string x, string mapsto){
	vector<ChaPair> out;
	ChaPair temp;
	for (size_t i = 0; i < x.length(); i++)
	{
		temp.x = x[i];
		temp.mapsTo = mapsto[i];
		out.push_back(temp);
	}
	return out;
}

//finds if 2 full or partial keys will not produce the same result
bool key_conflict(string key1, string key2){
	for (size_t i = 0; i < key1.length(); i++)
	{
		if (key1[i] != key2[i] && (key1[i] != '*' && key2[i] != '*')){
			//if the ith char of key1 is not the same as the ith char of key2
			//and the ith char of either is not a '*', then the keys are in conflict.
			return true;
		}
	}
	return false;
}

//returns a single key from a vector of incomplete keys
//which have been determined not to conflict.
string merge_keys(strvec keys){
	string out = "**************************";	//26 '*'s
	for (size_t i = 0; i < keys.size(); i++)
	{
		for (size_t j = 0; j < keys[i].length(); j++)
		{
			if (keys[i][j] != '*'){ out[j] = keys[i][j]; }
		}
	}
	return out;
}

string char_vector_to_string(vector<char> in){
	string s(in.begin(), in.end());
	return s;
}

//void depth_first_keygen(const strvec& encrMsg, const str_2D& grid){
//	strvec keys;
//	string root, msg, keycheck;
//	vector<int> map; //indecies of of the rows in each column where keys do not conflict
//	for (size_t i = 0; i < grid[0].size(); i++)//iterate through the rows in the first column
//	{
//		msg = encrMsg[i];
//		root = grid[0][i];
//		keycheck = generate_key(make_chapair_vec(msg, root));
//		for (size_t j = 1; j < grid.size(); j++)//iterate through the columns (but not the 0th one)
//		{
//			for (size_t k = 0; k < grid[j].size(); k++)//iterate through the rows in column j
//			{
//				if ()
//			}
//		}
//	}
//}

//pass the full encrypted message to this function.
//generates a list of possible (incomplete) keys
strvec solve_by_pattern(string message){
	strvec messageParsed;
	string temp;
	stringstream sst(message);
	while (sst >> temp){
		//each word is its own array element.
		messageParsed.push_back(temp);
	}

	//for each word, make a list of all words of the same pattern.
	str_2D matched;
	for (size_t i = 0; i < messageParsed.size(); i++)
	{
		matched.push_back(match_by_pattern(messageParsed[i]));
	}

	//make a list of possible keys for each word
	vector<list<string> >keys;
	list<string> templist;
	for (size_t i = 0; i < matched.size(); i++)
	{
		templist.clear();
		for (size_t j = 0; j < matched[i].size(); j++)
		{
			//cout << generate_key(make_chapair_vec(matched[i][j], messageParsed[i])) << endl;
			templist.push_back(generate_key(make_chapair_vec(matched[i][j], messageParsed[i])));
		}
		templist.sort();
		templist.unique();
		keys.push_back(templist);
	}


	cout << "Keys Found" << endl;

	str_2D tree;
	tree.resize(keys.size());
	for (size_t i = 0; i < tree.size(); i++)
	{
		tree[i].assign(begin(keys[i]), end(keys[i]));
	}

	/*
	//check if any keys conflict with each other. if so, remove them.(all keys must be checked)
	list<string>::iterator iter_i;
	list<string>::iterator iter_j;
	string comperand, comparitor;
	bool conflictFound = false;
	for (size_t i = 0; i < keys.size(); i++)
	{
	iter_i = keys[i].begin();
	while (iter_i != keys[i].end()){
	comperand = *iter_i;
	for (size_t j = 0; j < keys.size(); j++){
	if (j == i){ continue; }//don't check against keys in the same column.
	iter_j = keys[j].begin();
	while (iter_j != keys[j].end()){
	comparitor = *iter_j;
	conflictFound = key_conflict(comparitor, comperand);
	if (conflictFound){
	iter_j = keys[j].erase(iter_j);
	}
	else{
	++iter_j;
	}
	}
	}
	//dont forget to remove the first key if a conflict was found
	if (conflictFound){
	iter_i = keys[i].erase(iter_i);
	}
	else{
	++iter_i;
	}
	}
	}
	cout << "Keys culled." << endl;
	*/

	strvec out;

	for (size_t i = 0; i < keys.size(); i++){
		out.insert(end(out), begin(keys[i]), end(keys[i]));
	}

	return out;
}

strvec empatternate(strvec& input){
	strvec pattern_list;
	pattern_list.resize(Handler::LENGTH_OF_LIST);
	for (int i = 0; i < Handler::LENGTH_OF_LIST; i++){
		pattern_list[i] = char_pattern(input[i]);
	}
	return pattern_list;
}

void load_word_list(){
	init_words();
	patt = empatternate(wordlist);
}

int main(){
	load_word_list();
	string s = "AAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDDDDDDDEEEEEEEEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHHHHHIIIIIIIIIIIIIIIIIIJJJJJJJJJJJJJJJJJKKKKKKKKKKKKKKKKLLLLLLLLLLLLLLLMMMMMMMMMMMMMMNNNNNNNNNNNNNOOOOOOOOOOOOPPPPPPPPPPPQQQQQQQQQQRRRRRRRRRSSSSSSSSTTTTTTTUUUUUUVVVVVWWWWXXXYYZ";
	remove_dpw(s);
	//cout << s << endl;
	//shuffle(s);
	//cout << s << endl;
	//find_anagrams("");
	/*int num = 0;
	for (int i = 0; i < Handler::LENGTH_OF_LIST; i++){
		if (is_candidate(wordlist[i], s)){
			num++;
		}
	}*/

	print_histogram(s, 6);

	char c;
	cin >> c;
	return 0;
}