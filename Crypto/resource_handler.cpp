#include "resource_handler.h"

vector<string> temp;
vector<string> wordlist = temp;	//"definition"
vector<string> patt = temp;

struct simple_error{
public:
	simple_error(std::string s) :str(s){};
private:
	std::string str;
};

simple_error error(std::string s){
	throw simple_error(s);
}

Handler::Handler()
{
	data = new string[LENGTH_OF_LIST];
	getResource();
}

Handler::~Handler()
{
}

void Handler::extractResource()
{
	HGLOBAL     res_handle = NULL;
	HRSRC       res;
	DWORD       res_size;

	// NOTE: providing g_hInstance is important, NULL might not work
	res = FindResource(NULL, MAKEINTRESOURCE(IDR_TEXT1), "TEXT");
	if (!res)
		error("nope");
	res_handle = LoadResource(NULL, res);
	if (!res_handle)
		error("nope");
	res_data = (char*)LockResource(res_handle);
	res_size = SizeofResource(NULL, res);
	/* you can now use the resource data */
}

void Handler::getResource(){
	extractResource();
	int j = 0;
	for (int i = 0; i < LENGTH_OF_LIST; i++, j++){
		while (res_data[j] != '\n'){
			data[i] += res_data[j];
			j++;
		}

	}
}

string* Handler::getData(){
	return data;
}

string* open_resource(){
	Handler h;
	return h.getData();
}

void init_words(){
	vector<string> words;
	words.resize(Handler::LENGTH_OF_LIST);
	patt.resize(Handler::LENGTH_OF_LIST);
	string * temp = open_resource();
	copy(temp, temp + Handler::LENGTH_OF_LIST, words.begin());
	delete[] temp;
	wordlist = words;
	//return words;
}