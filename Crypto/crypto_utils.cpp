#include "crypto_utils.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

void capitalize(string& in){
	for (size_t i = 0; i < in.length(); i++){
		if (in[i] >= 'a' && in[i] <= 'z'){
			in[i] = (in[i] - 'a') + 'A';	//to upper case
		}
	}
}

//remove punctuation
void remove_p(string& in){
	capitalize(in);
	string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z') || (in[i] >= '0' && in[i] <= '9') || in[i] == ' '){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

//remove digits and punctuation
void remove_dp(string& in){
	capitalize(in);
	string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z') || in[i] == ' '){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

//remove digits, punctuation, and spaces
void remove_dpw(string& in){
	capitalize(in);
	string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z')){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

void shuffle(string& in){
	srand(time(0));
	int r = 0;
	char temp;
	for (int i = 0; i < in.length(); i++){
		r = rand() % in.length();
		temp = in[i];
		in[i] = in[r];
		in[r] = temp;
	}
}

void print_list(const vector<string>& in){
	for (string elem : in){
		cout << elem << ' ';
	}
}

string char_vector_to_string(vector<char> in){
	string s(in.begin(), in.end());
	return s;
}