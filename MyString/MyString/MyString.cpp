#include "MyString.h"
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstring>
using namespace std;

//tools
allocator<char> MyString::alloc;
void MyString::reallocate(size_type new_cap)
{
	auto newstr = alloc.allocate(new_cap);
	auto dst = newstr;
	auto sz = size();//store size into sz;cause for changes size();
	for (size_type i = 0; i != sz; ++i) {
		//lvalue to rvalue. move to new space not copy
		alloc.construct(dst++, std::move(*data_++));
	}
	//cout << "dst distance:" << dst - newstr << endl;
	
	data_ = newstr;
	end_ = dst;
	*end_ = '\0';
	cap_ = data_ + new_cap - 1;
	*cap_ = '\0';
}
pair<char*, char*> MyString::alloc_n_copy(const char *b, const char *e)
{
	auto ret = alloc.allocate(e - b + 1);
	return{ ret, uninitialized_copy(b,e,ret) };
}
void MyString::free()
{
	//use alloc to deallocate the allocated resource
	auto sz = capacity();
	while (cap_ != data_) {
		alloc.destroy(--cap_);
	}
	//return to system
	alloc.deallocate(data_, sz);
}

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
	*end_ = '\0';
	cap_ = end_;
}

//copy control functions:
MyString::MyString(const MyString &rhs)
{
	//use beg and end to allocate a new space 
	auto p = alloc_n_copy(rhs.cbegin(), rhs.cend());
	data_ = p.first;
	*p.second = '\0';//*p.second='\0' equals *p.second=(char)0,a null-terminated flag
	cap_ = end_ = p.second;
}

MyString& MyString::operator=(const MyString &rhs)
{
	//deal with self-assignment
	auto newstr = alloc_n_copy(rhs.cbegin(), rhs.cend());
	free();

	data_ = newstr.first;
	*newstr.second = '\0';
	cap_ = end_ = newstr.second;
	return *this;
}

MyString::~MyString()
{
	free();
}
//move control functions:
MyString::MyString(MyString &&rhs) noexcept:data_(rhs.data_),end_(rhs.end_),cap_(rhs.cap_)
{
	rhs.end_ = rhs.cap_ = rhs.data_ = nullptr;
}
MyString& MyString::operator=(MyString &&rhs) noexcept
{
	if (this != &rhs) {
		free();
		data_ = rhs.data_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;

		rhs.data_ = rhs.end_ = rhs.cap_ = nullptr;
	}
	return *this;
}

//public member functions:
char & MyString::at(size_type pos)
{
	if (pos >= size() || pos < 0) {
		throw out_of_range("at wrong position");
	}
	return *(data_ + pos);
}
const char & MyString::at(size_type pos) const
{
	if (pos >= size() || pos < 0) {
		throw out_of_range("at wrong position");
	}
	return *(data_ + pos);
}

char & MyString::operator[](size_type pos)
{
	if (pos >= size() || pos < 0) {
		throw out_of_range("operator[] wrong position");
	}
	return *(data_ + pos);
}

const char & MyString::operator[](size_type pos) const
{
	if (pos >= size() || pos < 0) {
		throw out_of_range("operator[] wrong position");
	}
	return *(data_ + pos);
}

//capacity functions:
void MyString::reserve(size_type new_cap)
{
	if (new_cap >= capacity()) {
		reallocate(new_cap);
	}
}

MyString::size_type MyString::max_size() noexcept
{
	return -1;
}

void MyString::clear()
{
	//clear all the elements in it, but don't deallocate the memory
	end_ = data_;
	*end_ = '\0';//terminate the string here;
}

MyString & MyString::insert(size_type index, size_type count, char ch)
{
	//check if new_cap greater than capacity
	chk_capacity(size() + count + 1);

	//move current elements behind and then insert into index
	for (size_type ix = size() - 1, cnt = 0; cnt != count; ++cnt) {
		data_[ix + count] = data_[ix];
		//cout << data_[ix + count] << endl;
		data_[ix] = ch;
		--ix;
	}
	end_ += count;
	return *this;
}


//friend functions:
ostream & operator<<(ostream &os, const MyString &str)
{
	const char* chr = str.c_str();
	while (*chr) {
		os << *chr++;
	}
	return os;
}
