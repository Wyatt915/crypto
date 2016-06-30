#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "graph.hpp"
#include "substitution.hpp"
#include "wordlist.hpp"
#include "process.hpp"

#include <list>
#include <algorithm>
#include <iostream>

namespace subst{
	std::string null_key(){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	}

	std::string empty_key(){
		return std::string(26, '*');
	}

	void invert(std::string &key){
		std::string inv = empty_key();
		for (int i = 0; i < 26; i++){
			if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
		}
		key = inv;
	}

	std::string invert(std::string key, bool a){
		std::string inv = empty_key();
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
				ciphertext += text[i]; //ignore non-alpha chars
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
	utils::VString match_by_pattern(std::string in){
		utils::VString out;
		std::string pattern = analyze::char_pattern(in);
		for (int i = 0; i < LENGTH_OF_LIST; i++)
		{
			if (pattern == PATTLIST[i]){ out.push_back(WORDLIST[i]); }
		}
		return out;
	}

	//sorts by length of word, from greatest to least
	bool sort_by_length(const std::string& lhs, const std::string& rhs){
		return lhs.length() > rhs.length();
	}

	//sorts a vector of vectors according to the size of each vector, greatest to least.
	bool sort_by_size(const utils::VString& lhs, const utils::VString& rhs){
		return lhs.size() > rhs.size();
	}

	//if there are more than [n] words, only keep the [n] longest.
	void largest_n(utils::VString& msg, int n){
		std::sort(msg.begin(), msg.end(), sort_by_length);
		if (msg.size() > n){
			utils::VString::iterator iter = msg.begin();
			std::advance(iter, n);
			msg.erase(iter, msg.end());
		}
	}

	//minimize the search space of the graph
	void optimize(utils::VVString& in){
		int max = 4096; //maximum search space
		int searchSpace = 0;
		utils::VVString::iterator iter = in.begin();
		while (iter != in.end()){
			//if there are no direct pattern matches, or there are too many to be practical, remove those columns.
			if (iter->size() == 0 || iter->size() > 300 || searchSpace > max){
				iter = in.erase(iter);
			}
			else{
				searchSpace += iter->size();
				iter++;
			}
		}
		//std::sort(in.begin(), in.end(), sort_by_size);
	}


	//pass the full encrypted message to this function.
	//generates a list of possible (incomplete) keys
	utils::VString solve_by_pattern(std::string cipher){
		utils::VString cipherParsed = utils::parse(cipher);

		largest_n(cipherParsed, 16);
		//print_list(messageParsed, ' ');

		unsigned long numkeys = 0;//just for debugging.
		//for each word, make a list of all words of the same pattern.
		utils::VVString matched;
		for (size_t i = 0; i < cipherParsed.size(); i++)
		{
			matched.push_back(match_by_pattern(cipherParsed[i]));
		}
		//make a list of possible keys for each word
		std::vector<std::list<std::string> >keys;
		std::list<std::string> templist;
		for (size_t i = 0; i < matched.size(); i++)
		{
			templist.clear();
			for (size_t j = 0; j < matched[i].size(); j++)
			{
				numkeys++;
				templist.push_back(process::generate_key(process::make_charPair_vec(matched[i][j], cipherParsed[i])));
			}
			templist.sort();
			templist.unique();
			keys.push_back(templist);
		}

		utils::VVString graphData;
		graphData.resize(keys.size());

		for (size_t i = 0; i < graphData.size(); i++)
		{
			graphData[i].assign(std::begin(keys[i]), std::end(keys[i]));
		}

		optimize(graphData);

		Graph g(graphData);
		utils::VString out = g.get_key_vec();
		return out;
	}

	//This function takes the encrypted message and the keys generated from
	//solve_by_pattern() as parameters, and attempts to further refine the
	//result by looking at partially solved words and comparing them to
	//WORDLIST, and then going through the same depth-first search as
	//solve_by_pattern() uses in graph.cpp
	void fine_solve(std::string cipher, std::string courseKey){
		std::string msg = decode(cipher, courseKey, true);

		utils::VString cipherParsed = utils::parse(cipher);
		utils::VString plainParsed = utils::parse(msg);

		utils::VString cipherPartials;
		utils::VString plainPartials;

		for (size_t j = 0; j < plainParsed.size(); j++) {
			if(!analyze::is_complete(plainParsed[j])){
				plainPartials.push_back(plainParsed[j]);
				cipherPartials.push_back(cipherParsed[j]);
			}
		}

		utils::VVString matched;
		std::cout << plainParsed.size() << std::endl;
		for (size_t i = 0; i < plainPartials.size(); i++) {
			matched.push_back(process::match_partial_words(plainPartials[i]));
		}
		//make a list of possible keys for each word
		std::vector<std::list<std::string> >keys;
		std::list<std::string> templist;
		std::string empty;
		for (size_t i = 0; i < matched.size(); i++)
		{
			templist.clear();
			for (size_t j = 0; j < matched[i].size(); j++)
			{
				templist.push_back(process::generate_key(process::make_charPair_vec(matched[i][j], cipherPartials[i])));
			}
			templist.sort();
			templist.unique();
			keys.push_back(templist);
		}

		utils::VVString graphData;
		graphData.resize(keys.size());

		for (size_t i = 0; i < graphData.size(); i++)
		{
			graphData[i].assign(std::begin(keys[i]), std::end(keys[i]));
		}
		optimize(graphData);

		for(int x = 0; x < graphData.size(); x++){
			utils::print_list(graphData[x]);
		}

		Graph g(graphData);
		utils::VString out = g.get_key_vec();
		utils::print_list(out, '\n');
	}
}
