#pragma once
#include<malloc.h>

template <size_t T>
class Alignment
{
public:

	static void* operator new(size_t i) { return _mm_malloc(i, T); }
	static void operator delete(void* p) {	_mm_free(p);}


};
