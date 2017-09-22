#include <iostream>
#include <string>

using namespace std;

int main()
{
	string word;
	int sec=0;
	cin >> word;
	if (2 <= word.length() && word.length() <= 15)
	{
		for (int i = 0; i < word.length(); i++)
		{
			if (65 <= word[i] && word[i] <= 67) sec += 3;
			else if (68 <= word[i] && word[i] <= 70) sec += 4;
			else if (71 <= word[i] && word[i] <= 73) sec += 5;
			else if (74 <= word[i] && word[i] <= 76) sec += 6;
			else if (77 <= word[i] && word[i] <= 79) sec += 7;
			else if (80 <= word[i] && word[i] <= 83) sec += 8;
			else if (84 <= word[i] && word[i] <= 86) sec += 9;
			else if (87 <= word[i] && word[i] <= 90) sec += 10;
			else return 0;
		}
		cout << sec << endl;
	}
	return 0;
}