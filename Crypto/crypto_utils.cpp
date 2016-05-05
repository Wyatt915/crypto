#include "crypto_utils.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

void capitalize(std::string& in){
	for (size_t i = 0; i < in.length(); i++){
		if (in[i] >= 'a' && in[i] <= 'z'){
			in[i] = (in[i] - 'a') + 'A';	//to upper case
		}
	}
}

//remove punctuation
void remove_p(std::string& in){
	capitalize(in);
	std::string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z') || (in[i] >= '0' && in[i] <= '9') || in[i] == ' '){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

//remove digits and punctuation
void remove_dp(std::string& in){
	capitalize(in);
	std::string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z') || in[i] == ' '){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

//remove digits, punctuation, and spaces
void remove_dpw(std::string& in){
	capitalize(in);
	std::string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z')){
			copy.push_back(in[i]);
		}
	}
	in = copy;
}

void shuffle(std::string& in){
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

void print_list(const std::vector<std::string>& in){
	for (std::string elem : in){
		std::cout << elem << ' ';
	}
}

void print_list(const std::vector<std::string>& in, char delim){
	for (std::string elem : in){
		std::cout << elem << delim;
	}
}
void print_list(const std::vector<std::string>& in, std::string delim){
	for (std::string elem : in){
		std::cout << elem << delim;
	}
}

std::string char_vector_to_string(std::vector<char> in){
	std::string s(in.begin(), in.end());
	return s;
}