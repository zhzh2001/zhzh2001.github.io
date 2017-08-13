#include <iostream>
#include <limits>
using namespace std;
int main()
{
	int n;
	cin >> n;
	double ans = .0;
	for (int i = 1; i <= n; i++)
		ans += 1.0 / (i * i);
	cout.precision(numeric_limits<double>::digits10);
	cout << fixed << ans << endl;
	return 0;
}