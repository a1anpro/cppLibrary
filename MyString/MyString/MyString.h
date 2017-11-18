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
	static const size_t npos = -1;
	friend ostream& operator<<(ostream &os, const MyString &);
	
	MyString();
	MyString(size_type sz, char chr);
	MyString(const char*);
	MyString(const char *, const char *);
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
	MyString& insert(size_type index, const char *str, size_type count = 1);

	MyString& erase(size_type index = 0, size_type len = npos);
	MyString& erase(char *position);
	MyString& erase(char *b, char *e);

	void push_back(char ch);
	void pop_back();

	MyString& append(size_type count, char ch);
	MyString& append(const MyString &rhs);

	MyString& operator+= (const MyString &rhs);
	MyString& operator+= (char ch);
	MyString& operator+= (const char *str);
	
	int compare(const MyString &rhs) const;

	MyString substr(size_type pos = 0, size_type count = npos) const;
	size_type copy(char *dest, size_type count, size_type pos = 0)const;
	void resize(size_type count, char ch = ' ');
	void swap(MyString &rhs);

	//search
	size_type find(const MyString &str, size_type pos = 0) const;

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

	char *data_;//start position of the string
	char *end_;//end postition of the string
	char *cap_;//store the total memory it has, behind the *end_
};

//declaration outside
ostream& operator<<(ostream &os, const MyString &);