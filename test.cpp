#include <iostream>
using namespace std;
#include <cstring>
int main(){

	string s="hello000sadfsdfsdf";
	
	cout << s.capacity() <<endl;
	s.clear();
	
	cout << s.capacity() <<endl;
	cout << s.size() <<endl;	
	s = "hello";
	cout << s.capacity() <<endl;
	cout << s.size() <<endl;
	return 0;
}
