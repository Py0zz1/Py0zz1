#include <iostream>
#include <string>
using namespace std;
int arr[26];
int main()
{
	string S;
	cin >> S;
	if (S.length() > 100) return 0;
	for(int i=0; i<26; i++)
	{
		arr[i] = S.find((char)(97 + i));
		if (S.find((char)(97 + i))== string::npos) arr[i] = -1;
	}
	for (int i = 0; i < 26; i++)
	{
		cout << arr[i] <<" ";
	}
	cout << endl;
	return 0;
}