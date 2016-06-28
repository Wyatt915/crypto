#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "graph.hpp"
#include "substitution.hpp"
#include "wordlist.h"
#include "process.hpp"

#include <list>


namespace subst{
	//Produces the key "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	std::string null_key(){
		char ary[26];
		for (int i = 0; i < 26; i++){
			ary[i] = i + 'A';
		}
		std::string s(ary, 26);
		return s;
	}

	void invert(std::string &key){
		std::string inv(26, '*');
		for (int i = 0; i < 26; i++){
			if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
		}
		key = inv;
	}

	std::string invert(std::string key, bool a){
		std::string inv(26, '*');
		for (int i = 0; i < 26; i++){
			if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
		}
		return inv;
	}


	void encode(std::string &text, const std::string &key){
		utils::capitalize(text);
		std::string ciphertext;

		for (size_t i = 0; i < text.length(); i++){
			if (!(text[i] >= 'A' && text[i] <= 'Z')){
				ciphertext += text[i]; //ignore non-26 chars
			}
			else{ text[i] = key[text[i] - 'A']; }
		}
	}

	void decode(std::string &text, std::string key){
		invert(key);
		encode(text, key);
	}

	std::string encode(std::string text, const std::string key, bool){
		utils::capitalize(text);
		std::string ciphertext;

		for (size_t i = 0; i < text.length(); i++){
			if (!(text[i] >= 'A' && text[i] <= 'Z')){
				ciphertext += text[i]; //ignore non-26 chars
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
	std::vector<std::string> match_by_pattern(std::string in){
		std::vector<std::string> out;
		std::string pattern = analyze::char_pattern(in);
		for (int i = 0; i < LENGTH_OF_LIST; i++)
		{
			if (pattern == patt[i]){ out.push_back(wordlist[i]); }
		}
		return out;
	}

	//if there are more than [n] words, only keep the 16 longest.
	void largest_n(std::vector<std::string>& msg, int n){
		if (msg.size() > n){
			std::vector<std::string> temp;
			unsigned int maxLen = 0;
			int maxLenIdx = 0; //where is the longest word
			for (size_t i = 0; i < n; i++)
			{
				for (size_t j = 0; j < msg.size(); j++){
					if (msg[j].length() > maxLen){
						maxLen = msg[j].length();
						maxLenIdx = j;
					}
				}
				temp.push_back(msg[maxLenIdx]);
				msg.erase(msg.begin() + maxLenIdx);
				maxLen = 0;
				maxLenIdx = 0;
			}
			msg = temp;
		}
	}

	//minimize the search space of the graph
	void optimize(std::vector<std::vector<std::string> >& in){
		size_t pos = 0;
		while (pos < in.size()){
			//if there are no direct pattern matches, or there are too many to be practical, remove those columns.
			if (in[pos].size() == 0 || in[pos].size() > 300){
				in.erase(in.begin() + pos);
				pos = 0;
			}
			else{ pos++; }
		}
	}


	//pass the full encrypted message to this function.
	//generates a list of possible (incomplete) keys
	std::vector<std::string> solve_by_pattern(std::string message){
		std::vector<std::string> messageParsed = utils::parse(message);

		largest_n(messageParsed, 16);
		//print_list(messageParsed, ' ');

		unsigned long numkeys = 0;//just for debugging.
		//for each word, make a list of all words of the same pattern.
		std::vector<std::vector<std::string> > matched;
		for (size_t i = 0; i < messageParsed.size(); i++)
		{
			matched.push_back(match_by_pattern(messageParsed[i]));
		}
		//make a std::list of possible keys for each word
		std::vector<std::list<std::string> >keys;
		std::list<std::string> templist;
		for (size_t i = 0; i < matched.size(); i++)
		{
			templist.clear();
			for (size_t j = 0; j < matched[i].size(); j++)
			{
				numkeys++;
				templist.push_back(process::generate_key(process::make_chapair_vec(matched[i][j], messageParsed[i])));
			}
			templist.sort();
			templist.unique();
			keys.push_back(templist);
		}

		std::vector<std::vector<std::string> > ragged;
		ragged.resize(keys.size());

		for (size_t i = 0; i < ragged.size(); i++)
		{
			ragged[i].assign(std::begin(keys[i]), std::end(keys[i]));
		}

		optimize(ragged);

		Graph g(ragged);
		std::vector<std::string> out = g.get_key_vec();
		return out;
	}
}
