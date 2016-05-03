#pragma once
#include <array>
#include <numeric>
#include <stdio.h>
#include <string>
#include "crypto_utils.h"

string char_pattern(string in);

struct ChaPair{
	char x, mapsTo;
};

//[in] must be UPPERCASE
array<int, 26> countchars(string in);
void print_histogram(string in, int resolution);
string char_vector_to_string(vector<char>);
bool key_conflict(string, string);
string merge_keys(string, string);
string merge_keys(vector<string>);
vector<ChaPair> make_chapair_vec(string, string);
string generate_key(vector<ChaPair>);

class Graph{
public:
	Graph(const std::vector<std::vector<string> >&);
	void dft(unsigned int, unsigned int);
	vector<string> getKeyList();
private:
	vector<string> keysFromPath;
	std::vector<int> path;
	vector<vector<int> > paths;
	std::vector<std::vector<string> > verts;
	string key;	//cumulative key being generated
	std::vector<string> keylist;
};