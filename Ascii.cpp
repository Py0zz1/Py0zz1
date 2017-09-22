#include <iostream>
using namespace std;

int main()
{
	char ascii;

	cin >> ascii;
	switch (ascii) {
	case 0: cout << 48 << endl;
		break;
	case 1: cout << 49 << endl;
		break;
	case 2: cout << 50 << endl;
		break;
	case 3: cout << 51 << endl;
		break;
	case 4: cout << 52 << endl;
		break;
	case 5: cout << 53 << endl;
		break;
	case 6: cout << 54 << endl;
		break;
	case 7: cout << 55 << endl;
		break;
	case 8: cout << 56 << endl;
		break;
	case 9: cout << 57 << endl;
		break;
	default: cout << (int)ascii<<endl;
	}
	

}