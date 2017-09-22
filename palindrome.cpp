#include <iostream>
#include <string>

using namespace std;

int main()
{
	string word,palindrome;
	
	cin >> word;
	if (word.length() > 100) return 0;
	int length = word.length();	
	while (length)
	{
		palindrome += word[length - 1];
		length--;
	}
	if (palindrome.compare(word) == 0) cout << 1 << endl;
	else cout << 0 << endl;

	return 0;
}