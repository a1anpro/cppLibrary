#include <iostream>
#include "MyString.h"
using namespace std;

int main() {
	MyString ms("hello");

	ms.insert(2, 4, 'x');
	cout << ms << endl;
	cout << ms.size() << endl;
	cout << ms.capacity() << endl;
	return 0;
}