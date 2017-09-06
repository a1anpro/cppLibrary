/*
Simplify Version1.0
Subject: String
Author:yanhui
Start Time:2017/9/5
*/

#pragma once
#include <memory>
#include <algorithm>
using namespace std;

class MyString {
public:
	typedef size_t size_type;

	friend ostream& operator<<(ostream &os, const MyString &);
	
	MyString();
	MyString(size_type sz, char chr);
	MyString(const char*);
	//copy control functions:
	MyString(const MyString&);
	MyString& operator=(const MyString&);
	~MyString();
	//move control functions:
	MyString(MyString &&) noexcept;
	MyString& operator=(MyString &&) noexcept;

	//member function
	bool empty() const {
		return size() == 0;
	}
	size_type size() const {
		return end_ - data_;
	}
	size_type length() const {
		return end_ - data_;
	}
	size_type capacity() const {
		return cap_ - data_ + 1;
	}
	const char* c_str() const {
		return data_;
	}
	char* begin() {
		return data_;
	}
	char* end() {
		return end_;
	}
	const char* cbegin() const {
		return data_;
	}
	const char* cend() const {
		return end_;
	}
	void reserve(size_type new_cap = 0);
	size_type max_size() noexcept;

	//operations function:
	void clear();
	MyString& insert(size_type index, size_type count, char ch);

	//elements access
	char& at(size_type pos);
	const char& at(size_type pos)const;//constant obj call it
	char& operator[](size_type pos);
	const char& operator[](size_type pos) const;

	char& front() {
		return operator[](0);
	}
	const char& front() const {
		return operator[](0);
	}
	char& back() {
		return operator[](size() - 1);
	}
	const char& back() const {
		return operator[](size() - 1);
	}

private:
	void reallocate(size_type new_cap);
	void chk_capacity(size_type new_cap) {
		if (new_cap > capacity()) {
			reallocate(new_cap);
		}
	}
	pair<char*, char*> alloc_n_copy(const char *beg, const char *end);
	void free();
private:
	//static tool:an allocator to allocate flexiblely
	static allocator<char> alloc;

	char *data_;
	char *end_;
	char *cap_;//store the total memory it has
};

//declaration outside
ostream& operator<<(ostream &os, const MyString &);