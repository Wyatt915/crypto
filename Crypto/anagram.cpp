#include "anagram.h"
#include "crypto_utils.h"
#include "analyze.h"

void sanitize(std::string& in){
	capitalize(in);
	remove_dpw(in);
}

//can a word, [test], be part of the anagram's solution?
//i.e. does [test] "fit" into [anagram]?
bool is_candidate(std::string test, std::string anagram){
	std::array<int, 26> test_count = countchars(test);
	std::array<int, 26> anagram_count = countchars(anagram);
	for (int i = 0; i < 26; i++){
		//if the test word has more of a certain letter
		//than the anagram, it cannot be part of the solution.
		if (test_count[i] > anagram_count[i]){
			return false;
		}
	}
	return true;
}
bool is_anagram(std::string test, std::string anagram){
	std::array<int, 26> test_count = countchars(test);
	std::array<int, 26> anagram_count = countchars(anagram);
	std::cout << '\n' << "OH BOY HERE I GO\n";
	for (int i = 0; i < 26; i++){
		std::cout << test_count[i] << ' ';
	}
	std::cout << '\n';

	for (int i = 0; i < 26; i++){
		if (test_count[i] != anagram_count[i]){
			return false;
		}
	}
	return true;
}

void find_anagrams(std::string i){
	if (is_candidate("CHARLESWYATT", "XXXXXCHARLESWYATTSHEFFIELD"))
		std::cout << "TRUE";
	else
		std::cout << "FALSE";

}