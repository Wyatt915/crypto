#include "analyze.h"
#include <algorithm>
#include <math.h>
#include <iostream>
//in must be UPPERCASE
array<int, 26> countchars(string in){
	array<int, 26> out;

	for (unsigned int i = 0; i < 26; i++){ out[i] = 0; }

	for (unsigned int i = 0; i < in.length(); i++){
		out[in[i] - 'A']++;
	}
	return out;
}

array<float, 26> frequency(string in){
	array<int, 26> count = countchars(in);
	int sum = accumulate(count.begin(), count.end(), 0);
	array<float, 26> freq;
	for (int i = 0; i < 26; i++){
		freq[i] = (float)count[i] / (float)sum;
	}
	return freq;
}

void print_histogram(string in){
	array<float, 26> freq = frequency(in);
	float max_val = *max_element(freq.begin(), freq.end());
	int resolution = 20;

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