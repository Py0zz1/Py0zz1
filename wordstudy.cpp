#include <iostream>
#include <string>

using namespace std;
int  arr[26]; // �迭 ������������ �ڵ�0���� �ʱ�ȭ
			 // ���������, ù��° �ε����� ���� ���� ������ �ʱ�ȭ �ȵ�
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

	for (int i = 0; i < 26; i++) // �ִ� ���ĺ� ã��
	{
		if (arr[i] > word_high)
		{
			word_high = arr[i];
			index = i;
		}
	}
	for (int i = 0; i < 26; i++) // �ߺ� ���ĺ� ã��
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
int *index = new int[length]; //�迭 �����Ҵ�

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