/****************************************
*										*
*	 Array with simple length value		*
*	Copyright © 2015 Wyatt Sheffield	*
*			  Version 1.0				*
*										*
****************************************/

#ifndef ARRAYL
#define ARRAYL

template<class T> class lst
{
public:
	lst();
	lst(size_t);
	~lst();
	T& operator[](size_t);
private:
	T * ary;
	size_t size;
};

template<class T> lst::lst()
{
	ary = new T[0];
}

template<class T> lst::lst(size_t s) :size(s)
{
	ary = new T[s];
}

template<class T> lst::~lst()
{
	delete[] ary;
}

template<class T>& lst::operator[](size_t idx){
	return &ary[idx];
}

#endif