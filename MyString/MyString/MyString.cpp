#include "MyString.h"
#include <memory>
#include <cstring>
using namespace std;

allocator<char> MyString::alloc;

MyString::MyString():data_(alloc.allocate(1))
{
	alloc.construct(data_, '\0');
	cap_ = end_ = data_;//cause no elements in it;
}

MyString::MyString(size_type sz, char chr):data_(alloc.allocate(sz + 1))
{
	end_ = data_;//data_ is the start pointer
	alloc.construct(end_++, chr);
	*end_ = '\0';//flag to end
	cap_ = end_;
}

MyString::MyString(const char *str):data_(alloc.allocate(strlen(str)+1))
{
	//*i or i?
	end_ = data_;
	for (auto i = str; *i != '\0'; ++i) {
		*end_++ = *i;//if *i is null-terminated, then end
	}
	cap_ = end_;
}

MyString::~MyString()
{
	//use alloc to deallocate the allocated resource
	auto sz = size();
	while (cap_ != data_) {
		alloc.destroy(--cap_);
	}
	//return to system
	alloc.deallocate(data_, sz);
}

ostream & operator<<(ostream &os, const MyString &str)
{
	const char* chr = str.c_str();
	os << *chr;
	return os;
}
