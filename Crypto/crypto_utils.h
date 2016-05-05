#pragma once
#include <string>
#include <vector>

using namespace std;

void capitalize(string& in);
void remove_p(string& in);
void remove_dp(string& in);
void remove_dpw(string& in);//remove digits, punctuation, and spaces
void shuffle(string& in);
void print_list(const vector<string>& in);
void print_list(const vector<string>& in, char);
void print_list(const vector<string>& in, string);
string char_vector_to_string(vector<char>);