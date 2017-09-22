#include <iostream>
#include <string>

using namespace std;

int main()
{
	string aeiou;
	int count=0;
	cin >> aeiou;
	for (int i = 0; i < aeiou.length(); i++) {
		if (aeiou.at(i) == 'a' ||
			aeiou.at(i) == 'e' ||
			aeiou.at(i) == 'i' ||
			aeiou.at(i) == 'o' ||
			aeiou.at(i) == 'u') count++;
	}
	cout << count << endl;


}