#pragma once
#include <list>
#include <string>
#include <vector>

class Graph{
public:
	Graph(const std::vector<std::vector<std::string> >&);
	int numcalls;
	std::list<std::string> get_key_list();
	std::vector<std::string> get_key_vec();
    bool debug = false;
private:
	std::list<std::string> keylist;
	std::string key;	//cumulative key being generated
	std::vector<int> path;
	std::vector<std::string> keysFromPath;
	std::vector<std::vector<int> > paths;
	std::vector<std::vector<std::string> > verts;
	void dft(unsigned int, unsigned int);
};
