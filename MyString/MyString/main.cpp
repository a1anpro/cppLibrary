#include <iostream>
#include "MyString.h"
#include <cassert>
using namespace std;

int main() {
	MyString str("hello");
	cout << str[1] << endl;

	/*const MyString str("hello,world");
		
	cout << str << endl;*/

	/*MyString ms("This is a string");

	auto n = ms.find("is");

	if (n == MyString::npos) {
		cout << "not found" << endl;
	}
	else {
		cout << ms.substr(n) << endl;
	}*/

	return 0;
}