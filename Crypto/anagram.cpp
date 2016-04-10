#include "anagram.h"
#include "crypto_utils.h"
#include "analyze.h"

void sanitize(string& in){
	capitalize(in);
	remove_dpw(in);
}

//can a word, [test], be part of the anagram's solution?
//i.e. does [test] "fit" into [anagram]?
bool is_candidate(string test, string anagram){
	array<int, 26> test_count = countchars(test);
	array<int, 26> anagram_count = countchars(anagram);
	for (int i = 0; i < 26; i++){
		//if the test word has more of a certain letter
		//than the anagram, it cannot be part of the solution.
		if (test_count[i] > anagram_count[i]){
			return false;
		}
	}
	return true;
}
bool is_anagram(string test, string anagram){
	array<int, 26> test_count = countchars(test);
	array<int, 26> anagram_count = countchars(anagram);
	cout << endl << "OH BOY HERE I GO\n";
	for (int i = 0; i < 26; i++){
		cout << test_count[i] << ' ';
	}
	cout << endl;

	for (int i = 0; i < 26; i++){
		if (test_count[i] != anagram_count[i]){
			return false;
		}
	}
	return true;
}

void find_anagrams(string i){
	if (is_candidate("CHARLESWYATT", "XXXXXCHARLESWYATTSHEFFIELD"))
		cout << "TRUE";
	else
		cout << "FALSE";

}