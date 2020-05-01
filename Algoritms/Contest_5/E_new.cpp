#include <iostream>

const int INF = 1000000000;


bool bit (int pos, int mask)	// Is bit 1 in mask on pos position
{
	return ((mask >> pos) & 1);
}


int main ()
{
	int i = 0;
	int j = 0;
	int n = 0;
	std::cin >> n;
	n++;
	int max_mask = (1 << n);
	int answer = INF;
	int cost[n][n] = {};
	int dp[max_mask][n] = {};
	int prev[max_mask][n] = {};

	for (i = 1; i < n; ++i)
	{
		for (j = 1; j < n; ++j)
			std::cin >> cost[i][j];
	}

	for (i = 1; i < n; ++i) cost[i][i] = 0;

	for (i = 0; i < max_mask; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			dp[i][j] = INF;
		}
	}

	dp[0][0] = 0;

	for (int mask = 0; mask < max_mask; ++mask)
	{
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if (bit (j, mask) && dp[mask ^ (1 << i)][j] + cost[i][j] < dp[mask][i])
				{
					dp[mask][i] = dp[mask ^ (1 << i)][j] + cost[i][j];
					prev[mask][i] = j;
				}
			}
		}
	}

	int min_ind = 0;
	
	for (i = 0; i < n; ++i)
	{
		if (dp[max_mask - 1][i] + cost[i][0] < answer)
		{
			answer = dp[max_mask - 1][i] + cost[i][0];
			min_ind = i;
		}
	}

	printf("%d\n", answer);

	int mask = max_mask - 1;
	i = min_ind;
	int tmp = 0;

	while (i != 0)
	{
		printf("%d ", i);
		tmp = i;
		i = prev[mask][i];
		mask = mask ^ (1 << tmp);
	}
	
	return 0;
}