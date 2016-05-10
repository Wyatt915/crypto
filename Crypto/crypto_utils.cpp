#include "crypto_utils.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <list>

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

//remove digits and punctuation, replace with whitespace
void remove_dp(std::string& in){
	capitalize(in);
	std::string copy;
	for (unsigned int i = 0; i < in.length(); i++){
		if ((in[i] >= 'A' && in[i] <= 'Z') || in[i] == ' '){
			copy.push_back(in[i]);
		}
		else{ copy.push_back(' '); }
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
	for (size_t i = 0; i < in.length(); i++){
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

//------------[Permutation functions]------------

int factorial(int x){
	int y = 1;
	for (int i = x; i > 0; i--){
		y *= i;
	}
	return y;
}

//copies each element of [in] [howmany] times
void duplicate(std::vector<std::list<char> > &in, int howmany){
	std::vector<std::list<char> > temp;
	for (size_t i = 0; i < in.size(); i++){
		for (int j = 0; j <= howmany; j++){
			temp.push_back(in[i]);
		}
	}
	in = temp;
}

//inserts [what] at every position in the duplicated elements
void weave(std::vector<std::list<char> > &in, char what, int mod){
	std::list<char>::iterator iter;
	for (size_t i = 0; i < in.size(); i++){
		iter = in[i].begin();
		std::advance(iter, (i%mod));
		in[i].insert(iter, what);
	}
}

//gives all permutations of the chars in [str]
std::vector<std::string> permute(std::string str){
	int len = str.length();
	int num = factorial(len);
	std::vector<std::list<char> > perm;
	perm.reserve(num);
	perm.push_back(std::list<char>(1, str[0])); //initialize
	for (int i = 0; i < len; i++){
		duplicate(perm, i);
		weave(perm, str[i], i + 1);
	}

	std::vector<std::string> out;
	for (int i = 0; i < num; i++){
		out.push_back(std::string(perm[i].begin(), perm[i].end()));
	}
	return out;
}