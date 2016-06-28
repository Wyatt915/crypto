#include "analyze.hpp"
#include "combo.hpp"
#include "crypto_utils.hpp"
#include "process.hpp"
#include "wordlist.h"

#include <iostream>

namespace process{

	std::string generate_key(std::vector<ChaPair> map){
		std::string out = std::string(26, '*');
		for (size_t i = 0; i < map.size(); i++)
		{
			out[map[i].x - 'A'] = map[i].mapsTo;
		}
		return out;
	}

	//x and mapsto will always be the same length
	std::vector<ChaPair> make_chapair_vec(std::string x, std::string mapsto){
		std::vector<ChaPair> out;
		ChaPair temp;
		for (size_t i = 0; i < x.length(); i++)
		{
			temp.x = x[i];
			temp.mapsTo = mapsto[i];
			out.push_back(temp);
		}
		return out;
	}

	//returns a single key from a std::vector of incomplete keys
	//which have been determined not to conflict.
	std::string merge_keys(std::vector<std::string> keys){
		std::string out = std::string(26, '*');	//26 '*'s
		for (size_t i = 0; i < keys.size(); i++)
		{
			for (size_t j = 0; j < keys[i].length(); j++)
			{
				if (keys[i][j] != '*'){
					out[j] = keys[i][j];
				}
			}
		}
		return out;
	}

	std::string merge_keys(std::list<std::string> l){
		std::vector<std::string> keys{ make_move_iterator(begin(l)), make_move_iterator(end(l)) };
		std::string out = std::string(26, '*');	//26 '*'s
		for (size_t i = 0; i < keys.size(); i++)
		{
			for (size_t j = 0; j < keys[i].length(); j++)
			{
				if (keys[i][j] != '*'){
					out[j] = keys[i][j];
				}
			}
		}
		return out;
	}

	std::string merge_keys(std::string key1, std::string key2){
		std::string out = std::string(26, '*');	//26 '*'s
		for (int i = 0; i < 26; i++){
			if (key1[i] != '*'){
				out[i] = key1[i];
			}
		}
		for (int i = 0; i < 26; i++){
			if (key2[i] != '*'){
				out[i] = key2[i];
			}
		}
		return out;
	}

	int factorial(int n){
		int f = 1;
		for(int i = n; i > 1; i--){
			f *= i;
		}
		return f;
	}

	int nPr(int n, int r){
		return factorial(n) / factorial(n-r);
	}

	std::vector<std::string> fill_blanks(std::string ciphertext, std::string plaintext, std::string key){
		std::vector<std::string> out;
		std::vector<int> blankSpots;

		std::vector<bool> u = analyze::used_unknowns(ciphertext, plaintext);
		int numMissing = 0;
		for (int i = 0; i < 26; i++){
			if (u[i]){
				blankSpots.push_back(i);
				numMissing++;
			}
		}
		std::cout << "There are " << nPr(analyze::missing_key_values(key).length(), numMissing) << " permutations to check. Continue (y/n)? ";
		char choice;
		std::cin >> choice;
		if(!(choice == 'Y' || choice == 'y')){ std::exit(0); }
		std::vector<std::string> perms = combos(numMissing, analyze::missing_key_values(key));

		for (size_t i = 0; i < perms.size(); i++){
			out.push_back(key);
			for (int j = 0; j < numMissing; j++){
				out[i][blankSpots[j]] = perms[i][j];
			}
		}

		return out;
	}

	bool is_partial(std::string s){
		for (size_t i = 0; i < s.length(); i++) {
			if(s[i] == '*'){ return true; }
		}
		return false;
	}

	void match_partial_words(std::string part){
		std::list<std::string> m;
		for(size_t i = 0; i < LENGTH_OF_LIST; i++){
			if(wordlist[i].length() == part.length()){
				m.push_back(wordlist[i]);
			}
		}
		std::list<std::string>::iterator iter = m.begin();
		std::string current;
		for(size_t i = 0; i < part.length(); i++){
			std::cout << "Elements: " << m.size() << std::endl;
			if(part[i] == '*'){ continue ;}
			else{
				while(iter != m.end()){
					current = *iter;
					if(current[i] != part[i]){ iter =  m.erase(iter); }
					else{ ++iter; }
				}

			}
			iter = m.begin();
		}
		std::vector<std::string> v{ make_move_iterator(begin(m)), make_move_iterator(end(m)) };
		utils::print_list(v, '\n');
	}
}
