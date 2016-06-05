#pragma once
#include <array>
#include <numeric>
#include <stdio.h>
#include <string>
#include <list>
#include "crypto_utils.h"



struct ChaPair{
	char x, mapsTo;
};

class Graph{
public:
	Graph(const std::vector<std::vector<std::string> >&);
	std::list<std::string> get_key_list();
	std::vector<std::string> get_key_vec();
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

std::string char_pattern(std::string in);
std::array<int, 26> count_chars(std::string in); //[in] must be UPPERCASE
void print_histogram(std::string in, int resolution);
std::string char_vector_to_string(std::vector<char>);
bool key_conflict(std::string, std::string);
std::string merge_keys(std::string, std::string);
std::string merge_keys(std::vector<std::string>);
std::vector<ChaPair> make_chapair_vec(std::string, std::string);
std::string generate_key(std::vector<ChaPair>);
std::string missing_key_values(std::string);
std::vector<std::string> fill_blanks(std::string);