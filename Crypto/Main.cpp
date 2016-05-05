#define GVARS	//for the wordlist in resource_handler.h

#include "crypto_utils.h"
#include "resource_handler.h"
#include "anagram.h"
#include "analyze.h"
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

str_2D sort_by_length(strvec in){
	size_t len = 0;
	for (int i = 0; i < Handler::LENGTH_OF_list; i++){	//get maximum word length
		if (in[i].length() > len){ len = in[i].length(); }
	}
	len++;
	int *temp = new int[len];
	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s

	for (int i = 0; i < Handler::LENGTH_OF_list; i++){
		temp[in[i].length()]++;	//get number of words for each length
	}

	str_2D out(len);

	for (size_t i = 0; i < len; i++){
		out[i].resize(temp[i]); //initialize 2d array in the form [length][index_of_word]
	}

	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s
	//temp is now used to keep track of #words in each row
	for (int i = 0; i < Handler::LENGTH_OF_list; i++){
		out[in[i].length()][temp[in[i].length()]] = in[i];
		temp[in[i].length()]++;
	}
	delete[] temp;
	return out;
}

//returns a list of all words of the same pattern
strvec match_by_pattern(std::string in){
	strvec out;
	std::string pattern = char_pattern(in);
	for (size_t i = 0; i < Handler::LENGTH_OF_list; i++)
	{
		if (pattern == patt[i]){ out.push_back(wordlist[i]); }
	}
	return out;
}

//if there are more than 16 words, only keep the 16 longest.
void optimize(strvec& in){
	if (in.size() > 16){
		strvec temp;
		int maxLen = 0;
		int maxLenIdx = 0; //where is the longest word
		for (size_t i = 0; i < 16; i++)
		{
			for (size_t j = 0; j < in.size(); j++){
				if (in[j].length() > maxLen){
					maxLen = in[j].length();
					maxLenIdx = j;
				}
			}
			temp.push_back(in[maxLenIdx]);
			in.erase(in.begin() + maxLenIdx);
			maxLen = 0;
			maxLenIdx = 0;
		}
		in = temp;
	}
}

//minimize the search space of the graph
void optimize(str_2D& in){
	size_t pos = 0;
	while (pos < in.size()){
		//if there are no direct pattern matches, or there are too many to be practical, remove those columns.
		//also if the length of the word is too short.  longer words provide more info.
		if (in[pos].size() == 0 || in[pos].size() > 250){
			in.erase(in.begin() + pos);
			pos = 0;
		}
		else{ pos++; }
	}
}


//pass the full encrypted message to this function.
//generates a list of possible (incomplete) keys
strvec solve_by_pattern(std::string message){
	strvec messageParsed;
	std::string temp;
	std::stringstream sst(message);
	while (sst >> temp){
		//each word is its own array element.
		messageParsed.push_back(temp);
	}

	optimize(messageParsed);
	std::cout << "\n\n";
	print_list(messageParsed, ' ');
	std::cout << "\n\n";

	unsigned long numkeys = 0;//just for debugging.
	//for each word, make a list of all words of the same pattern.
	str_2D matched;
	for (size_t i = 0; i < messageParsed.size(); i++)
	{
		matched.push_back(match_by_pattern(messageParsed[i]));
	}
	std::cout << "finding keys...";
	//make a std::list of possible keys for each word
	std::vector<std::list<std::string> >keys;
	std::list<std::string> templist;
	for (size_t i = 0; i < matched.size(); i++)
	{
		templist.clear();
		for (size_t j = 0; j < matched[i].size(); j++)
		{
			numkeys++;
			//std::cout << generate_key(make_chapair_vec(matched[i][j], messageParsed[i])) << '\n';
			templist.push_back(generate_key(make_chapair_vec(matched[i][j], messageParsed[i])));
		}
		templist.sort();
		templist.unique();
		keys.push_back(templist);
	}


	std::cout << numkeys << "Keys Found...Making graph...";

	str_2D ragged;
	ragged.resize(keys.size());
	for (size_t i = 0; i < ragged.size(); i++)
	{
		ragged[i].assign(begin(keys[i]), end(keys[i]));
	}

	optimize(ragged);

	Graph g(ragged);
	std::cout << "Graph created...searching...";
	strvec out = g.getKeyVec();
	std::cout << "DONE.\n\n";
	return out;
}

strvec empatternate(strvec& input){
	strvec pattern_list;
	pattern_list.resize(Handler::LENGTH_OF_list);
	for (int i = 0; i < Handler::LENGTH_OF_list; i++){
		pattern_list[i] = char_pattern(input[i]);
	}
	return pattern_list;
}

void load_word_list(){
	init_words();
	patt = empatternate(wordlist);
}
void test(std::string in){
	std::string key = "QWERTYUIOPASDFGHJKLZXCVBNM";
	remove_dp(in);	//sanitize
	//std::cout << in << '\n';
	//encode(in, key);
	std::cout << in << '\n';
	strvec keys = solve_by_pattern(in);
	strvec messages;
	std::string temp;
	for (size_t i = 0; i < keys.size(); i++){
		temp = decode(in, keys[i], true);
		messages.push_back(temp);
	}
	print_list(messages, "\n\n");
}

int main(){
	std::cout << "Loading wordlist...";
	load_word_list();
	std::cout << "DONE.\n\n";

	std::string s = "BMBU SUWKMNMOUN LXS VXBOUBOUH TMYMBR DUQTOD UBXKRD OX FQZU GXSZ Q ITUQNKSU GUQTOD UBXKRD OX NKIIXSO EXKS BUUHN NOSUBROD OX JQOOTU GMOD HMLLMVKTOMUN QBH XYUSVXFU ODUF RSQVU UBXKRD OX VXBLUNN EXKS NMBN QBH LXSNQZU ODUF IQOMUBVU UBXKRD OX OXMT KBOMT NXFU RXXH MN QVVXFITMNDUH VDQSMOE UBXKRD OX NUU NXFU RXXH MB EXKS BUMRDJXKS TXYU UBXKRD OX FXYU EXK OX JU KNULKT QBH DUTILKT OX XODUSN LQMOD UBXKRD OX FQZU SUQT ODU ODMBRN XL RXH DXIU UBXKRD OX SUFXYU QTT QBAMXKN LUQSN VXBVUSBMBR ODU LKOKSU";

	test(s);

	std::cout << "DONE. ";
	char c;
	std::cin >> c;
	return 0;
}