#include <iostream>
#include <string>
using namespace std;

int main()
{
	int count,length;
	string binary,binary_2,binary_sum,oct;

	cin >> binary;
	if(binary.length() % 3 != 0) {
		count = binary.length() % 3;
		for (count; count < 3; count++) binary_2 += "0";
	}
	binary_sum = binary_2 + binary;
	length = binary_sum.length();
	for (int i = 0; i <= length;) {
		oct.assign(binary_sum,0, 3);
		if (!oct.compare("000")) cout << 0;
		else if (!oct.compare("001")) cout << 1;
		else if (!oct.compare("010")) cout << 2;
		else if (!oct.compare("011")) cout << 3;
		else if (!oct.compare("100")) cout << 4;
		else if (!oct.compare("101")) cout << 5;
		else if (!oct.compare("110")) cout << 6;
		else if (!oct.compare("111")) cout << 7;
		binary_sum.erase(0, 3);
		i += 3;
	}
	cout << endl;
	return 0;
}