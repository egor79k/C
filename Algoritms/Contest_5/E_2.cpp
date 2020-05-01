#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int INF = 2000000000;


int main ()
{
	ios::sync_with_stdio (0);
	cin.tie (0);

	int answer = INT_MAX;
	int i = 0;
	int j = 0;
	int n = 0;
	cin >> n;
	const int max_mask = 1 << n;
	int cost[n][n] = {};

	int **dp = (int **) calloc (max_mask, sizeof (int *));
	for (i = 0; i < max_mask; ++i) dp[i] = (int *) calloc (n, sizeof (int));

	for (i = 0; i < max_mask; ++i)
		for (j = 0; j < n; ++j)
			dp[i][j] = INF;
	
	for (i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			cin >> cost[i][j];
	
	for (i = 0; i < n; ++i)
		dp[0][i] = 0;
	
	for (int mask = 0; mask < max_mask; ++mask)
	{
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if (mask & (1 << i))
				{
					if (__builtin_popcount (mask) == 1)
						dp[mask][i] = 0;
					else
						dp[mask][i] = min (dp[mask][i], dp[mask ^ (1 << i)][j] + cost[j][i]);
				}
			}
		}
	}
	
	
	int min_ind = 0;

	for (i = 0; i < n; ++i)
		if (answer > dp[max_mask - 1][i])
		{
			answer = dp[max_mask - 1][i];
			min_ind = i;
		}
	
	printf("%d\n", answer);

	int mask = max_mask - 1;
	int k = 0;
	int cheapest_path[n] = {};
	cheapest_path[k++] = min_ind;
	int n_ones = n - 1;
	i = min_ind;
	
	while (n_ones)
	{
		for (j = 0; j < n; ++j)
		{
			if (mask & (1 << j) && dp[mask][i] == dp[mask ^ (1 << i)][j] + cost[j][i])
			{
				cheapest_path[k++] = j;
				mask ^= (1 << i);
				n_ones--;
				i = j;
			}
		}
	}
	
	for (i = n - 1; i >= 0; --i)
		printf("%d ", cheapest_path[i] + 1);

	for (i = 0; i < n; ++i) free (dp[i]);
	free (dp);
}
