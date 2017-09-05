/*
Simplify Version1.0
Subject: String
Author:yanhui
Start Time:2017/9/5
*/

#pragma once
#include <memory>
using namespace std;

class MyString {
public:
	typedef size_t size_type;

	friend ostream& operator<<(ostream &os, const MyString &);

	MyString();
	MyString(size_type sz, char chr);
	MyString(const char*);

	//member function
	size_type size() const {
		return end_ - data_;
	}
	size_type capacity() const {
		return cap_ - data_;
	}
	const char* c_str() const {
		return data_;
	}
	~MyString();
private:
	//static tool:an allocator to allocate flexiblely
	static allocator<char> alloc;

	char *data_;
	char *end_;
	char *cap_;//store the total memory it has
};

//declaration outside
ostream& operator<<(ostream &os, const MyString &);