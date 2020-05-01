#include <iostream>

using namespace std;


int n = 0;
int max_mask = 0;
int **cost = 0;
int **dp = 0;
const int INF = 100000000;


bool bit (int mask, int pos)
{
	return (mask >> pos) & 1;
}


int FindCheapest (int i, int mask)
{
	if (dp[i][mask] != INF) return dp[i][mask];

	for (int j = 0; j < n; ++j)
	{
		if (bit (mask, j))
			dp[i][mask] = min (dp[i][mask], FindCheapest (j, mask ^ (1 << j)) + cost[i][j]);
	}

	return dp[i][mask];
}


void FindWay ()
{
	int i = 0;
	int mask = max_mask - 1;
	while (mask != 0)
	{
		for (int j = 0; j < n; ++j)
		{
			if (bit (mask, j) && dp[i][mask] == dp[j][mask ^ (1 << j)] + cost[i][j])
			{
				if (j) printf("%d ", j);
				i = j;
				mask = mask ^ (1 << j);
			}
		}
	}
	return;
}


int main ()
{
	int i = 0;
	cin >> n;
	n++;
	max_mask = 1 << n;

	cost = (int **) calloc (n, sizeof (int *));
	for (i = 0; i < n; ++i) cost[i] = (int *) calloc (n, sizeof (int));

	dp = (int **) calloc (n, sizeof (int *));
	for (i = 0; i < n; ++i) dp[i] = (int *) calloc (max_mask, sizeof (int));

	for (i = 1; i < n; ++i)
	{
		for (int j = 1; j < n; ++j) cin >> cost[i][j];
	}

	int max_mask = 1 << n;

	for (i = 0; i < n; ++i)
	{
		for (int mask = 0; mask < max_mask; ++mask)
		{
			dp[i][mask] = INF;
		}
	}

	dp[0][0] = 0;

	printf("%d\n", FindCheapest (0, max_mask - 1));

	for (i = 0; i < max_mask; ++i)
	{
		for (int j = 0; j < n; ++j)
			printf ("%10d ", dp[j][i]);
		printf("\n");
	}
	printf("\n");

	FindWay ();

	return 0;
}