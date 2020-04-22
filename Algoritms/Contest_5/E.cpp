#include <iostream>
#include <climits>

using namespace std;


int n = 3;
int cost[10][10] = {{8,1,6},
					{3,5,7},
					{4,9,2}};
int dp[10][1 << 10] = {};
const int INF = 100000000;
int asimpt = 0;

bool bit (int mask, int pos)
{
	return (mask >> pos) & 1;
}
//__builtin_popcount (mask);


int FindCheapest (int i, int mask)
{
	if (dp[i][mask] != INF) return dp[i][mask];

	for (int j = 0; j < n; ++j)
	{asimpt++;
		if (i != j && bit (mask, j))
			dp[i][mask] = min (dp[i][mask], FindCheapest (j, mask - (1 << j)) + cost[i][j]);
	}

	return dp[i][mask];
}


void FindWay ()
{
	int solve[n] = {};
	int count = 0;
	int max_cost = 0;
	int i = 0;
	int mask = (1 << n) - 1;
	//printf("0 ");
	while (mask != 0)
	{
		for (int j = 0; j < n; ++j)
		{
			if (bit (mask, j) && dp[i][mask - (1 << j)] + cost[i][j])
			{
				solve[count++] = j;
				printf("%d ", j + 1);
				i = j;
				mask = mask - (1 << j);
			}
		}
	}
	printf("\n");

	for (i = 0; i < n; ++i)
	{
		printf("%d (%d) ", solve[i], cost[solve[i]][(i + 1) % n]);
		if (cost[solve[i]][(i + 1) % n] > cost[solve[max_cost]][(max_cost + 1) % n]) max_cost = i;
	}

	printf("\nMAX_COST: %d\n\n", cost[solve[max_cost]][(max_cost + 1) % n]);

	for (i = max_cost + 1; i <= max_cost + n; ++i)
	{
		printf("%d ", solve[i % n] + 1);
	}
	return;
}


int main ()
{
	cin >> n;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) cin >> cost[i][j];

	int max_mask = 1 << n;

	for (int i = 0; i < n; ++i)
	{
		for (int mask = 0; mask < max_mask; ++ mask)
		{
			dp[i][mask] = INF;
		}
	}

	dp[0][0] = 0;

	printf("\nAnswer: %d\n", FindCheapest (0, max_mask - 1));
	printf("%d\n", asimpt);
	FindWay ();

/*	for (int i = 0; i < n; ++i)
	{
		for (int mask = 0; mask < max_mask; ++mask)
		{
			cout << dp[i][mask] << ' ';
		}
		cout << '\n';
	}*/
	return 0;
}