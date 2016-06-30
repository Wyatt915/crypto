#include "analyze.hpp"
#include "combo.hpp"
#include "wordlist.hpp"
#include "process.hpp"
#include "crypto_utils.hpp"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <sstream>

namespace analyze{
	/**
	*  Counts the number of instances of each character from A to Z.
	*  @param in must be UPPERCASE
	*/
	std::array<int, 26> count_chars(std::string in){
		std::array<int, 26> out;

		for (unsigned int i = 0; i < 26; i++){ out[i] = 0; }

		for (unsigned int i = 0; i < in.length(); i++){
			out[in[i] - 'A']++;
		}
		return out;
	}

	/**
	*  Finds the frequency of occurance of each letter in a given std::string.
	*/
	std::array<float, 26> frequency(std::string in){
		std::array<int, 26> count = count_chars(in);
		int sum = std::accumulate(count.begin(), count.end(), 0);
		std::array<float, 26> freq;
		for (int i = 0; i < 26; i++){
			freq[i] = (float)count[i] / (float)sum;
		}
		return freq;
	}

	/**
	*  Prints a histogram of letter frequency.
	*  @param in std::string to be analyzed.
	*  @param resolution the height of the graph, in lines.
	*/
	void print_histogram(std::string in, int resolution){
		std::array<float, 26> freq = frequency(in);
		float max_val = *std::max_element(freq.begin(), freq.end());

		for (int i = 0, j = resolution; i < resolution; i++, j--){
			std::cout << ' ';
			for (int k = 0; k < 26; k++){
				if ((freq[k] * (resolution / max_val)) >= j){
					std::cout << "# ";
				}
				else{
					std::cout << "  ";
				}
			}
			//create labels for the vertical axis.
			std::cout << "| ";
			printf("%.3f", 100 * (max_val - (((float)i / resolution)*max_val)));
			std::cout << "%\n";
		}

		std::cout << ' ';
		for (int i = 0; i < 26; i++){
			std::cout << (char)('A' + i) << ' ';
		}
		std::cout << '\n';
	}

	/**
	*  Produces the character pattern of a given word.
	*  For example, the input "ENCRYPTS" would give the output "********" because
	*  each character in the word is unique, i.e. none of the letters repeat. In contrast,
	*  The word "QUANTITATIVE" results in a pattern of "**A*BCBABC**" because the letter A
	*  is the first letter that gets repeated, and it occurs in the 3rd and 8th positions.
	*  Next, N is repeated, and since it is the second unique letter that is repeated, it is
	*  called B in the pattern output. The second repeat letter occurs at positions 5, 7, and 9.
	*
	*  The reason for using the letters A, B, C...Z (in order) to represent other letters
	*  is because if a word goes through a substitution cipher, its resultant pattern will
	*  not change. For instance the pattern of "AGRICULTURAstd::listS" is "A*BC*DEFDBAECGFG". If
	*  If AGRICULTURAstd::listS is passed through a substitution cipher, it will come out as something
	*  like QUKOEXSZXKQSOLZL, but the pattern will remain as "A*BC*DEFDBAECGFG".
	*/
	std::string char_pattern(std::string in){
		utils::capitalize(in);
		const int l = in.length();
		char *pattern = new char[l];
		bool *completed = new bool[l];
		char marker = 'A';
		bool match_found = false;

		//initialize the pattern
		for (int i = 0; i < l; i++){ pattern[i] = '*'; completed[i] = false; }

		for (int i = 0; i < l; i++){
			if (completed[i]){ continue; }
			if (in[i] == '-' || in[i] == '\''){
				pattern[i] = in[i];
				completed[i] = true;
			}
			else{
				//look at the ith index of the input and find all other occurances of it.
				//when all occurences have been found, set completed[] to true at each position of the occurance.
				//mark each index in pattern[] with marker, increment marker, increment i, and loop
				for (int j = i + 1; j < l; j++){
					if ((in[i] == in[j]) && !completed[j]){
						pattern[i] = marker;
						pattern[j] = marker;
						completed[j] = true;
						match_found = true;
					}
				}
				if (match_found){ marker++; }
				match_found = false;
			}
		}
		std::string out(pattern, l);
		delete[] pattern;
		delete[] completed;

		return out;
	}

	//finds if 2 full or partial keys will not produce the same result
	bool key_conflict(std::string key1, std::string key2){
		for (size_t i = 0; i < key1.length(); i++)
		{
			if (key1[i] != key2[i] && (key1[i] != '*' && key2[i] != '*')){
				//if the ith char of key1 is not the same as the ith char of key2
				//and the ith char of either is not a '*', then the keys are in conflict.
				return true;
			}

		}
		std::string t1, t2;
		t1 = key1;
		t2 = key2;
		t1 = process::merge_keys(t1, t2);
		utils::remove_dpw(t1);
		bool accounted[26] = { 0 };
		for (int i = 0; i < t1.length(); i++){
			//if the char at i has already been accounted for then
			//it is a duplicate and therefore the keys conflict.
			if (accounted[t1[i] - 'A']){ return true; }
			else { accounted[t1[i] - 'A'] = true; }
		}
		return false;
	}

	bool key_conflict(std::list<std::string> keys){
		std::string key1, key2;
		key1 = *keys.begin();
		std::list<std::string>::iterator iter = keys.begin();
		++iter;
		while (iter != keys.end()){
			key2 = *iter;
			for (size_t i = 0; i < key1.length(); i++)
			{
				if (key1[i] != key2[i] && (key1[i] != '*' && key2[i] != '*')){
					//if the ith char of key1 is not the same as the ith char of key2
					//and the ith char of either is not a '*', then the keys are in conflict.
					return true;
				}

			}
			std::string t1, t2;
			t1 = key1;
			t2 = key2;
			t1 = process::merge_keys(t1, t2);
			utils::remove_dpw(t1);
			bool accounted[26] = { 0 };
			for (int i = 0; i < t1.length(); i++){
				//if the char at i has already been accounted for then
				//it is a duplicate and therefore the keys conflict.
				if (accounted[t1[i] - 'A']){ return true; }
				else { accounted[t1[i] - 'A'] = true; }
			}
			key1 = process::merge_keys(key1, key2);
		}
		return false;
	}

	std::string missing_key_values(std::string k){
		//if k is not 26 chars long throw an error.
		std::string missing = "";
		bool accounted[26] = { 0 };
		for (int i = 0; i < 26; i++){
			if (k[i] != '*'){ accounted[k[i] - 'A'] = true; }
		}
		for (int i = 0; i < 26; i++){
			if (!accounted[i]){
				missing += (char)(i + 'A');
			}
		}
		return missing;
	}

	//returns an array of bools (one for each letter). True if the letter exists in
	//the ciphertext and maps to an unknown letter in the plaintext.
	std::vector<bool> used_unknowns(std::string ciphertext, std::string plaintext){
		std::vector<bool> unknowns(26, false);
		for (size_t i = 0; i < ciphertext.length(); i++) {
			if(plaintext[i] == '*'){
				unknowns[ciphertext[i] - 'A'] = true;
			}
		}
		return unknowns;
	}

	int prob_score(std::string plaintext){
		utils::VString parsed = utils::parse(plaintext);
		int idx = 0;
		int score = 0;

		for (size_t i = 0; i < parsed.size(); i++) {
			idx = utils::binary_search(parsed[i]);
			if(idx > -1){
				score += FREQLIST[idx];
			}
			else{
				score -= 2 * AVERAGE_FREQ;
			}
		}
		return score;
	}

	//true if there are no *s in the word
	bool is_complete(std::string word){
		for(int i = 0; i < word.length(); i++){
			if(word[i] == '*'){ return false; }
		}
		return true;
	}

	//true if [word] exists in WORDLIST
	bool is_known(std::string word){
		int idx = utils::binary_search(word);
		if(idx > -1){ return true; }
		return false;
	}
}
