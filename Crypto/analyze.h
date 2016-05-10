#pragma once
#include <array>
#include <numeric>
#include <stdio.h>
#include <string>
#include <list>
#include "crypto_utils.h"

std::string char_pattern(std::string in);

struct ChaPair{
	char x, mapsTo;
};

//[in] must be UPPERCASE
std::array<int, 26> countchars(std::string in);
void print_histogram(std::string in, int resolution);
std::string char_vector_to_string(std::vector<char>);
bool key_conflict(std::string, std::string);
std::string merge_keys(std::string, std::string);
std::string merge_keys(std::vector<std::string>);
std::vector<ChaPair> make_chapair_vec(std::string, std::string);
std::string generate_key(std::vector<ChaPair>);

class Graph{
public:
	Graph(const std::vector<std::vector<std::string> >&);
	std::list<std::string> getKeylist();
	std::vector<std::string> getKeyVec();
	int numcalls;
private:
	void dft(unsigned int, unsigned int);
	std::vector<std::string> keysFromPath;
	std::vector<int> path;
	std::vector<std::vector<int> > paths;
	std::vector<std::vector<std::string> > verts;
	std::string key;	//cumulative key being generated
	std::list<std::string> keylist;
};