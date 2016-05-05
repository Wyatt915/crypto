/*
*	Using code modified from http://stackoverflow.com/a/1994653
*/

#pragma once
#include "resource.h"
#include <Windows.h>
#include <string>
#include <vector>



//----------[Project Global Variables]-----------------------------------------

//#ifdef GVARS
//#define EXTERN
//#else
//#define EXTERN extern
//#endif
//
//EXTERN std::vector<std::string> wordlist;
//EXTERN std::vector<std::string> patt;

extern std::vector<std::string> wordlist;
extern std::vector<std::string> patt;
extern const unsigned int LENGTH_OF_list;

//----------[End of Global Variable Declaration]-------------------------------

class Handler
{
public:
	Handler();
	~Handler();
	void getResource();
	std::string* getData();
	static const size_t LENGTH_OF_list = 149544;
private:
	void extractResource();
	char *      res_data;
	std::string* data;
};

void init_words();