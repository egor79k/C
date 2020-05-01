#include <iostream>

using namespace std;

const int inf=1E9,NMAX=16;
int n, i, j, k, m, temp, ans, cost[NMAX][NMAX], dp[1<<NMAX][NMAX];


bool bit (int pos, int mask)	// Is bit 1 in mask on pos position
{
	return ((mask >> pos) & 1);
}


int main()
{
	cin >> n;

	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j) cin >> cost[i][j];

	int max_mask = 1 << n; // 2^n
	dp[1][0] = 0;

	for (int mask = 1; mask < max_mask; ++mask)
		for (j = (mask == 1) ? 1 : 0; j < n; ++j)
		{
			dp[mask][j] = inf;

			if (j > 0 && bit (j, mask))
			{
			temp = mask ^ (1 << j);	// Set of mask without j-th element

			for (k = 0; k < n; ++k)
				if (bit (k, mask) && cost[k][j] > 0)
					dp[mask][j] = min (dp[mask][j], dp[temp][k] + cost[k][j]);
			}
		}

	for (i = 0; i < max_mask; ++i)
	{
		for (j = 0; j < n; ++j)
			printf ("%10d ", dp[i][j]);
		printf("\n");
	}
	i = 0;
	int mask = max_mask - 1;
	printf("\n");


	ans = inf;
	for (j = 1; j < n; ++j)
		if (cost[j][0] > 0) ans = min (ans, dp[max_mask - 1][j] + cost[j][0]);

	if (ans == inf) cout << -1;
	else cout << ans;
}