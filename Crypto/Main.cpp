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

//struct Graph{
//	std::vector<int> path;
//	vector<vector<int> > paths;
//	std::vector<std::vector<string> > verts;
//	string key;
//	strvec keylist;
//};



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

	//trim the tree
	int i = 0;
	while (i < tree.size()){
		if (tree[i].size() == 0 || tree[i].size() > 250){
			tree.erase(tree.begin() + i);
			i = 0;
		}
		else{
			i++;
		}
	}

	Graph g(tree);

	g.dft(0, 0);

	strvec out = g.getKeyList();

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
	string s = "A disadvantage of this method of derangement is that the last letters of the alphabet (which are mostly low frequency) tend to stay at the end";
	remove_dp(s);
	cout << s << endl;
	encode(s, "QWERTYUIOPASDFGHJKLZXCVBNM");
	cout << s << endl;
	//print_list(solve_by_pattern(s));
	//string str = "ROLQRCQFZQUT";
	//decode(str, "Q**RT*U*O****F****LZ*C****");
	//cout << "\n\n" << str << endl;
	cout << "DONE. ";
	char c;
	cin >> c;
	return 0;
}