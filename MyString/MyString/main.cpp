#include <iostream>
#include "MyString.h"
#include <cassert>
using namespace std;

int main() {
	MyString ms("This is a string");

	auto n = ms.find("is");

	if (n == MyString::npos) {
		cout << "not found" << endl;
	}
	else {
		cout << ms.substr(n) << endl;
	}

	return 0;
}