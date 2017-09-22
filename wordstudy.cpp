#include <iostream>
#include <string>

using namespace std;
int  arr[26]; // 배열 전역선언으로 자동0으로 초기화
			 // 지역선언시, 첫번째 인덱스에 값을 주지 않으면 초기화 안됨
int main()
{
	string word;
	int count = 0, word_high = 0, index;
	cin >> word;
	for(int i = 0; i < word.length(); i++)
	{
		if (65 <= word[i] && word[i] <= 90) arr[word[i] - 65]++;
		else if (97 <= word[i] && word[i] <= 122) arr[word[i] - 97]++;
	}

	for (int i = 0; i < 26; i++) // 최다 알파벳 찾기
	{
		if (arr[i] > word_high)
		{
			word_high = arr[i];
			index = i;
		}
	}
	for (int i = 0; i < 26; i++) // 중복 알파벳 찾기
	{
		if (arr[i] == word_high)
		{
			count++;
			if (count >= 2)
			{
				cout << "?" << endl;
				return 0;
			}
		}
	}
	cout << (char)(index+65) << endl;
	return 0;
}
/*
 int length,count=0,count_high=0,check=0;

length = word.length();
if (length >= 1000000) return 0;
int *index = new int[length]; //배열 동적할당

for (int i = 0; i < length; i++) {
for (int j = i + 1; j < length; j++) {
if (word.at(i) == word.at(j)) count++;
}
if (count_high <= count) count_high = count;
index[i] = count;
count = 0;
}
for (int i = 0; i <= length; i++) {
if (count_high == index[i]) {
check++;
if (check >= 2)
{
cout << "?" << endl;
break;
}
if (word.at(i) > 90) word.at(i) -= 32;
count = i;
}
if (i == length) cout << word[count] << endl;
}
delete[] index;

return 0;

*/