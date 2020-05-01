#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int MAX_COST = 2000000000;


int builtin_popcount (int mask)						// Number of 1 in binary value
{
	int num = 0;
	while (mask)
	{
		mask &= (mask - 1);
		num++;
	} 
	return num;
}


bool bit (int pos, int mask)						// Is bit 1 in mask on pos position
{
	return ((mask >> pos) & 1);
}


int main ()
{
	ios::sync_with_stdio (0);
	cin.tie (0);

	int answer = INT_MAX;
	int min_ind = 0;
	int i = 0;
	int j = 0;
	int n = 0;

	cin >> n;

	const int max_mask = 1 << n;
	int cost[n][n] = {};

	std::vector<std::vector<int>> dp (max_mask, std::vector<int> (n, MAX_COST));
	
	for (i = 0; i < n; ++i) dp[0][i] = 0;
	
	for (i = 0; i < n; ++i)							// Costs initialization
		for (int j = 0; j < n; ++j)
			cin >> cost[i][j];
	

	
	for (int mask = 0; mask < max_mask; ++mask)		// DP counting
	{
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if (bit (i, mask))
				{
					if (builtin_popcount (mask) == 1) dp[mask][i] = 0;
					else dp[mask][i] = min (dp[mask][i], dp[mask ^ (1 << i)][j] + cost[j][i]);
				}
			}
		}
	}


	for (i = 0; i < n; ++i)							// Cheapest path length counting
	{
		if (answer > dp[max_mask - 1][i])
		{
			answer = dp[max_mask - 1][i];
			min_ind = i;
		}
	}

	printf("%d\n", answer);
	

	int mask = max_mask - 1;
	int k = 0;
	int cheapest_path[n] = {};
	cheapest_path[k++] = min_ind;
	int count = n - 1;
	i = min_ind;
	
	while (count)									// Cheapest path finding
	{
		for (j = 0; j < n; ++j)
		{
			if (bit (j, mask) && dp[mask][i] == dp[mask ^ (1 << i)][j] + cost[j][i])
			{
				cheapest_path[k++] = j;
				mask ^= (1 << i);
				i = j;
				count--;
			}
		}
	}
	
	for (i = n - 1; i >= 0; --i) printf("%d ", cheapest_path[i] + 1);

	return 0;
}
