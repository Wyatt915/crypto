/*
*	Using code modified from http://stackoverflow.com/a/1994653
*/

#pragma once
#include "resource.h"
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

//----------[Project Global Variables]-----------------------------------------

//#ifdef GVARS
//#define EXTERN
//#else
//#define EXTERN extern
//#endif
//
//EXTERN vector<string> wordlist;
//EXTERN vector<string> patt;

extern vector<string> wordlist;
extern vector<string> patt;

//----------[End of Global Variable Declaration]-------------------------------

class Handler
{
public:
	Handler();
	~Handler();
	void getResource();
	string* getData();
	static const size_t LENGTH_OF_LIST = 149544;
private:
	void extractResource();
	char *      res_data;
	string* data;
};

void init_words();