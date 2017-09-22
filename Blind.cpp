#include <iostream>
#include <string>
using namespace std;
char word[100][100];
int main()
{
	int x, y;
	int blind[5] = { 0 };
	cin >> x >> y;
	for (int i = 0; i < (x * 5 +1); i++)
	{
		scanf("%s", word[i]);
		
	}

	for (int i = 1; i < (5*x); )
	{
		for (int j = 1; j < (5*y); )
		{
			if(word[i][j] == '*') 
			{
				if (word[i+1][j] == '*')
				{
					if (word[i+2][j] == '*')
					{
						if (word[i + 3][j] == '*') blind[4]++;
						else if (word[i + 3][j] == '.')  blind[3]++; 
					}
					else if (word[i + 2][j] == '.') blind[2]++;
				}
				else if (word[i + 1][j] == '.') blind[1]++; 
			}
			else if (word[i][j] == '.') blind[0]++; 
			j += 5;
		}
		i += 5;
	}
	for(int i=0; i<5; i++)
	{
		cout << blind[i] << " ";
	}
	cout << endl;
	
}
/*
#include <iostream>
#include <string>
using namespace std;
char word[100][100];
int main()
{
int x, y;
int blind[5] = { 0 };
cin >> x >> y;
//y = y * 5 + 1;
//x = x * 5 + 1;
for (int i = 0; i < (x * 5 +1); i++)
{
scanf("%s", word[i]);

}

for (int i = 1; i < (5*x); )
{
for (int j = 1; j < (5*y); )
{
if(word[i][j] == '*')
{
if (word[i+1][j] == '*')
{
if (word[i+2][j] == '*')
{
if (word[i + 3][j] == '*') { blind[4]++; cout << "!!4번 ++!!" << i << j <<endl; }
else if (word[i + 3][j] == '.') { blind[3]++; cout << "!!3번 ++!!" << i << j << endl; }
}
else if (word[i + 2][j] == '.') { blind[2]++; cout << "!!2번 ++!!" << i << j << endl; }
}
else if (word[i + 1][j] == '.') { blind[1]++; cout << "!!1번 ++!!" << i << j << endl; }
}
else if (word[i][j] == '.') {
blind[0]++; cout << "!!0번 ++!!" << i << j << endl;
}

j += 5;
}
i += 5;
}
for(int i=0; i<5; i++)
{
cout << blind[i] << " ";
}
cout << endl;

}
*/