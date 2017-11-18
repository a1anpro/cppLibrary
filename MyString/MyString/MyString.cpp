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

MyString::MyString(const char *b, const char *e)
{
	auto p = alloc_n_copy(b, e);
	data_ = p.first;
	*p.second = '\0';
	cap_ = end_ = p.second;
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
	return const_cast<char&>(static_cast<const MyString&>(*this)[pos]);
	/*if (pos >= size() || pos < 0) {
		throw out_of_range("operator[] wrong position");
	}
	return *(data_ + pos);*/
}

const char & MyString::operator[](size_type pos) const
{
	//cout << "const×ª³ý µ÷ÓÃ²âÊÔ" << endl;
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
	size_type num = size() - index;
	for (size_type ix = size() - 1, cnt = 0; cnt != num; ++cnt) {
		data_[ix + count] = data_[ix];
		--ix;
	}
	for (size_type i = 0; i != count; ++i) {
		data_[index++] = ch;
	}

	end_ += count;
	return *this;
}

MyString & MyString::insert(size_type index, const char * str, size_type count)
{
	size_type total_count = strlen(str ) * count;
	chk_capacity(total_count + size() + 1);
	size_type num = size() - index;

	for (size_type ix = size() - 1, cnt = 0; cnt != num; ++cnt) {
		data_[ix + total_count] = data_[ix];//move behind total_count step;
		--ix;
	}
	for (size_type cnt = 0; cnt != count; ++cnt) {
		size_type length = strlen(str);
		for (size_type i = 0; i != length; ++i) {
			data_[index++] = *str++;
		}
	}
	end_ += total_count;
	return *this;
}

MyString & MyString::erase(size_type index, size_type len)
{
	if (len == npos) {
		end_ = data_ + index;
		*end_ = '\0';
	}
	else {
		size_type cnt = size() - index - len;
		for (size_type i = 0; i != cnt; ++i) {
			data_[index + i] = data_[index + len + i];
		}
		//cout << *this << endl;
		end_ = data_ + (size() - len);
		*end_ = '\0';
	}
	return *this;
}

MyString & MyString::erase(char * position)
{
	for (; position != end(); ++position) {
		*position = *(position + 1);
	}
	--end_;;
	*end_ = '\0';
	return *this;
}

MyString & MyString::erase(char * b, char * e)
{
	size_type len = e - b;
	for (auto i = e; i != end(); ++i) {
		*b = *i;
		++b;
	}
	end_ -= len;
	*end_ = '\0';
	return *this;
}

void MyString::push_back(char ch)
{
	chk_capacity(size() + 2);
	*end_++ = ch;
	*end_ = '\0';
}

void MyString::pop_back()
{
	--end_;
	*end_ = '\0';
}

MyString & MyString::append(size_type count, char ch)
{
	chk_capacity(count + size() + 1);
	for (size_type i = 0; i != count; ++i) {
		*(end_ + i) = ch;
	}
	end_ += count;
	*end_ = '\0';
	return *this;
}

MyString & MyString::append(const MyString & rhs)
{
	chk_capacity(rhs.size() + size() + 1);
	for (size_type i = 0; i != rhs.size(); ++i) {
		*end_++ = rhs[i];
	}
	*end_ = '\0';
	return *this;
}

MyString & MyString::operator+=(const MyString & rhs)
{
	append(rhs);
	return *this;
}

MyString & MyString::operator+=(char ch)
{
	push_back(ch);
	return *this;
}

MyString & MyString::operator+=(const char *str)
{
	insert(size(), str);
	return *this;
}

int MyString::compare(const MyString & rhs) const
{
	//lexicographical order
	auto min_ = (size() < rhs.size()) ? size() : rhs.size();
	
	for (decltype(min_)i = 0; i != min_; ++i) {
		if (data_[i] < rhs.data_[i]) {
			return -1;
		}
		else if (data_[i] > rhs.data_[i]){
			return 1;
		}
	}
	if (size() == rhs.size()) return 0;
	return size() < rhs.size() ? -1 : 1;
}

MyString MyString::substr(size_type pos, size_type count) const
{
	if (count == npos) {
		return MyString(data_ + pos, end_);
	}
	else {
		return MyString(data_ + pos, data_ + pos + count);
	}
}

MyString::size_type MyString::copy(char * dest, size_type count, size_type pos) const
{
	for (size_type i = 0; i != count; ++i) {
		dest[i] = data_[pos + i];
	}
	return count;
}

void MyString::resize(size_type count, char ch)
{
	if (count <= size()) {
		end_ -= size() - count;
		*end_ = '\0';
	}
	else {
		reallocate(count + 1);
		for (auto cnt = count - size(); cnt != 0; --cnt) {
			push_back(ch);
		}
	}
}

void MyString::swap(MyString & rhs)
{
	//swap the pointer only.
	auto temp_data = data_, temp_end = end_, temp_cap = cap_;
	data_ = rhs.data_;
	end_ = rhs.end_;
	cap_ = rhs.cap_;
	rhs.data_ = temp_data;
	rhs.end_ = temp_end;
	rhs.cap_ = temp_cap;
}

MyString::size_type MyString::find(const MyString & str, size_type pos) const
{
	//using strstr to find a substr
	auto p = strstr(data_ + pos, str.data_);
	if (p != nullptr) {
		//find the str in *this
		return p - data_ + pos;
	}
	else{
		return npos;
	}
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
