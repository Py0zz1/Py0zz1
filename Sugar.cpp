#include <iostream>
using namespace std;
int main()
{
	int N = 0, count = 0;
	cin >> N;
	if ((3<=N) & (N <= 5000)) {
		if (N % 5 == 0) {
			cout << (N / 5);
			return;
		}
		else if (N % 3 == 0) {
			cout << (N / 3);
			return;
		}


	}
	return 0;
}