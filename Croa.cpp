#include <iostream>
#include <string>

using namespace std;

int main()
{
	string word;
	int count = 0;
	cin >> word;
	if (word.length() > 100) return 0;
	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] == 'c' || word[i] == 'd' || word[i] == 'l' || word[i] == 'n' || word[i] == 's' || word[i] == 'z' || word[i] == '=' || word[i] == '-')
		{
			// c 眉农
			if (word[i] == 'c')
			{
				if (word[i + 1] == '=')
				{
					count++;
					i++;
				}
				else if (word[i + 1] == '-')
				{
					count++;
					i++;
				}
				else count++;
			}
			// d 眉农
			if (word[i] == 'd')
			{
				if (word[i + 1] == 'z')
				{
					if (word[i + 2] == '=')
					{
						count++;
						i += 2;
					}
					else count ++;
				}
				

				else if (word[i + 1] == '-')
				{
					count++;
					i++;
				}
				else count++;
			}

			// l / n 眉农
			if (word[i] == 'l' || word[i] == 'n')
			{
				if (word[i + 1] == 'j') {
					i++;
					count++;
				}
				else count++;
			}
			// s / z 眉农
			if (word[i] == 's' || word[i] == 'z')
			{
				if (word[i + 1] == '=')
				{
					i++;
					count++;
				}
				else count++;
			}
		}
		else if ((word[i] != 'q' && word[i] != 'w' && word[i] != 'x' && word[i] != 'y') && (97 <= word[i] && word[i] <= 122)) count++;

	}
	cout << count << endl;
}
/*
#include <iostream>
#include <string>

using namespace std;

int main()
{
string word;
int count = 0;
cin >> word;
if (word.length() > 100) return 0;
for (int i = 0; i < word.length(); i++)
{
cout << "\n柳青 : " << word[i] << endl;
if (word[i] == 'c' || word[i] == 'd' || word[i] == 'l' ||word[i] =='n'|| word[i]=='s' || word[i] =='z'||word[i] == '=' || word[i] == '-')
{
if (word[i] == 'c')
{
if (word[i + 1] == '=')
{
cout << "! c= !" << count;
count++;
i++;
}
else if (word[i + 1] == '-')
{
cout << "! c- !++" <<count;
count++;
i++;
}
else count++;
}
if (word[i] == 'd')
{
if (word[i + 1] == 'z')
{
if (word[i + 2] == '=')
{
cout << "! dz= ! ++" << count;
count++;
i += 2;
}
}
else if (word[i + 1] == '-')
{
cout << "! d- ! ++ " << count;
count++;
i++;
}
else count++;
}


if (word[i] == 'l' || word[i] =='n')
{
if (word[i+1] == 'j') {
i++;
count++;
cout << "! lj / nj !" << count;
}
else count++;
}
if (word[i] == 's' || word[i] =='z')
{
if (word[i+1] == '=')
{
i++;
count++;
cout << "! s= / z= !" << count;
}
else count++;
}

}
else if ((word[i] != 'q' && word[i] != 'w' && word[i] != 'x' && word[i] != 'y') && (97 <= word[i] && word[i] <= 122))
{

count++;
cout << "count!!" << count;
}
}
cout << count << endl;


}
*/