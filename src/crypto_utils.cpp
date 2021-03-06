#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "wordlist.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <math.h>
#include <vector>
#include <sstream>

crypto_error::crypto_error(std::string err){
    what = err;
}


namespace utils{

typedef std::vector<std::string> VString;
typedef std::vector<std::vector<std::string> > VVString;

bool consecutive_spaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

//returns the index of [what] in [WORDLIST].  returns -1 for an unsucessful search
int binary_search(std::string what){
    int lower_bound = 0;
    int upper_bound = WORDLIST.size() - 1;
    int pivot = 0;
    while(lower_bound <= upper_bound){
        pivot = floor((upper_bound + lower_bound)/2);
        if(WORDLIST[pivot] < what){
            lower_bound = pivot + 1;
            continue;
        }
        if(WORDLIST[pivot] > what){
            upper_bound = pivot - 1;
            continue;
        }
        if(WORDLIST[pivot] == what){ return pivot; }
    }
    return -1;
}

std::vector<std::string> parse(std::string message){
    //remove consecutive spaces
    std::string::iterator new_end = std::unique(message.begin(), message.end(), consecutive_spaces);
    message.erase(new_end, message.end());

    std::vector<std::string> out;
    std::string temp;
	std::stringstream sst(message);
	while (sst >> temp){
		//each word is its own array element.
		out.push_back(temp);
	}
    return out;
}

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

void sanitize(std::string &in) {
	capitalize(in);
	std::string copy;
	for (size_t i = 0; i < in.length(); i++) {
		if ((in[i] >= 'A' && in[i] <= 'Z') || in[i] == ' '  || in[i] == '\''){ //|| in[i] == '-'
			copy.push_back(in[i]);
		}
		else{ copy.push_back(' '); }
	}

    std::string::iterator new_end = std::unique(copy.begin(), copy.end(), consecutive_spaces);
    copy.erase(new_end, copy.end());

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

void print_color(std::string s){
	std::vector<std::string> parsed = parse(s);
	std::string red = "\x1b[31;1m";
	std::string def = "\x1b[0m";  //default
	std::string blu = "\x1b[34;1m";
	for(int i = 0; i < parsed.size(); i++){
		if(!analyze::is_complete(parsed[i])){
			parsed[i] = blu + parsed[i] + def;
		}
		else if(!analyze::is_known(parsed[i])){
			parsed[i] = red + parsed[i] + def;
		}
	}
	print_list(parsed);
}

void print_list(const std::vector<std::string>& in){
	for (std::string elem : in){
		std::cout << elem << ' ';
	}
    std::cout << std::endl;
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

//copies each element of [in] [howmany] times
void duplicate(std::list<std::list<char> > &in, int howmany){
	std::list<std::list<char>>::iterator iter = in.begin();
	while (iter != in.end()){
		for (int j = 0; j < howmany; j++){
			in.insert(iter, *iter);
		}
		iter++;
	}
}

//inserts [what] at every position in the duplicated elements	AB		[C]AB
//That is, it inserts [what] along a diagonal, "weaving" it		AB  ->	A[C]B
//into the array of values to give every permutation			AB		AB[C]
void weave(std::list<std::list<char> > &in, char what, int mod){
	std::list<char>::iterator charIter;
	std::list<std::list<char>>::iterator listIter = in.begin();
	for (size_t i = 0; i < in.size(); i++){
		charIter = listIter->begin();
		std::advance(charIter, (i % mod));
		*listIter->insert(charIter, what);
		listIter++;
	}
}

//gives all permutations of the chars in [str]
std::vector<std::string> permute(std::string str){
	//initialize the 0th list with a list of length 1 containing the 0th char of the input
	std::list<std::list<char> > perm;
	perm.push_back(std::list<char>(1, str[0]));

	for (int i = 1; i < str.length(); i++){
		duplicate(perm, i);
		weave(perm, str[i], i + 1);
	}

	std::list<std::list<char>>::iterator listIter = perm.begin();
	std::list<std::string> temp;
	while (listIter != perm.end()){
		temp.push_back(std::string(listIter->begin(), listIter->end()));
		listIter++;
	}
	std::vector<std::string> out{	std::make_move_iterator(std::begin(temp)),
									std::make_move_iterator(std::end(temp)) };

	return out;
}

//------------[Currently unused]------------

// std::vector<std::vector<std::string> > sort_by_length(std::vector<std::string> in){
// 	size_t len = 0;
// 	for (int i = 0; i < LENGTH_OF_LIST; i++){	//get maximum word length
// 		if (in[i].length() > len){ len = in[i].length(); }
// 	}
// 	len++;
// 	int *temp = new int[len];
// 	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s
//
// 	for (int i = 0; i < LENGTH_OF_LIST; i++){
// 		temp[in[i].length()]++;	//get number of words for each length
// 	}
//
// 	std::vector<std::vector<std::string> > out(len);
//
// 	for (size_t i = 0; i < len; i++){
// 		out[i].resize(temp[i]); //initialize 2d array in the form [length][index_of_word]
// 	}
//
// 	for (size_t i = 0; i < len; i++){ temp[i] = 0; }	//reset temp to be filled with 0s
// 	//temp is now used to keep track of #words in each row
// 	for (int i = 0; i < LENGTH_OF_LIST; i++){
// 		out[in[i].length()][temp[in[i].length()]] = in[i];
// 		temp[in[i].length()]++;
// 	}
// 	delete[] temp;
// 	return out;
// }
}
