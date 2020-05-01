#include <iostream>

int main ()
{
	int left = 0;
	int right = 0;
	int tmp_k = 0;
	int tmp_right = 0;
	int result = 0;
	int i = 0;
	int j = 0;
	int n = 0;
	int k = 0;
	std::cin >> n;
	std::cin >> k;
	char string[n] = {};

	for (i = 0; i < n; ++i)
		std::cin >> string[i];

	for (i = 0; i < n; ++i)
	{
		for (right = i; right < i + 2; ++right)
		{
			tmp_right = right;
			left = i;
			tmp_k = k;
			while (left >= 0 && right < n)
			{
				if (string[left] != string[right])
				{
					if (tmp_k > 0)
					{
						tmp_k--;
					}
					else break;
				}
				result++;
				left--;
				right++;
			}
			right = tmp_right;
		}
	}

	printf("%d", result);

	return 0;
}