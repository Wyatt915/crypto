#include "analyze.h"
#include <algorithm>
#include <math.h>
#include <iostream>


/**
*  Counts the number of instances of each character from A to Z.
*  @param in must be UPPERCASE
*/
array<int, 26> countchars(string in){
	array<int, 26> out;

	for (unsigned int i = 0; i < 26; i++){ out[i] = 0; }

	for (unsigned int i = 0; i < in.length(); i++){
		out[in[i] - 'A']++;
	}
	return out;
}

/**
*  Finds the frequency of occurance of each letter in a given string.
*/
array<float, 26> frequency(string in){
	array<int, 26> count = countchars(in);
	int sum = accumulate(count.begin(), count.end(), 0);
	array<float, 26> freq;
	for (int i = 0; i < 26; i++){
		freq[i] = (float)count[i] / (float)sum;
	}
	return freq;
}

/**
*  Prints a histogram of letter frequency.
*  @param in String to be analyzed.
*  @param resolution the height of the graph, in lines.
*/
void print_histogram(string in, int resolution){
	array<float, 26> freq = frequency(in);
	float max_val = *max_element(freq.begin(), freq.end());

	for (int i = 0, j = resolution; i < resolution; i++, j--){
		cout << ' ';
		for (int k = 0; k < 26; k++){
			if ((freq[k] * (resolution / max_val)) >= j){
				cout << "# ";
			}
			else{
				cout << "  ";
			}
		}
		//create labels for the vertical axis.
		cout << "| ";
		printf("%.3f", 100*(max_val - (((float)i / resolution)*max_val)));
		cout << "%\n";
	}

	cout << ' ';
	for (int i = 0; i < 26; i++){
		cout << (char)('A' + i) << ' ';
	}
	cout << '\n';
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
*  not change. For instance the pattern of "AGRICULTURALISTS" is "A*BC*DEFDBAECGFG". If
*  If AGRICULTURALISTS is passed through a substitution cipher, it will come out as something
*  like QUKOEXSZXKQSOLZL, but the pattern will remain as "A*BC*DEFDBAECGFG".
*/
string char_pattern(string in){
	capitalize(in);
	const int l = in.length();
	char *pattern = new char[l];
	bool *completed = new bool[l];
	char marker = 'A';
	bool match_found = false;

	//initialize the pattern
	for (int i = 0; i < l; i++){ pattern[i] = '*'; completed[i] = false; }

	for (int i = 0; i < l; i++){
		//look at the ith index of the input and find all other occurances of it.
		//when all occurences have been found, set completed[] to true at each position of the occurance.
		//mark each index in pattern[] with marker, increment marker, increment i, and loop
		if (completed[i]){ continue; }
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

	string out(pattern, l);
	delete[] pattern;
	delete[] completed;

	return out;
}