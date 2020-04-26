#include <iostream>
#include "Blocks.h"

int main ()
{
	/*for (int i = 0; i <= 140; ++i)
	{
		printf("\e[0m%3d - %s\n", i, BLOCKS[i]);
	}*/
	//::::ORANGE::::
	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 33; color < 100; color += 60)
		{
			for (int back = 41; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
			}
			printf("\n");
		}
	}

	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 31; color < 100; color += 60)
		{
			for (int back = 43; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
				printf("\n");
			}
		}
	}
	//::::BROWN::::
	printf("\n");
	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 32; color < 100; color += 60)
		{
			for (int back = 41; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
			}
			printf("\n");
		}
	}

	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 31; color < 100; color += 60)
		{
			for (int back = 42; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
				printf("\n");
			}
		}
	}
	//::::
	printf("\n");
	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 34; color < 100; color += 60)
		{
			for (int back = 43; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
			}
			printf("\n");
		}
	}

	for (int type = 0; type < 3; type += 2)
	{
		for (int color = 33; color < 100; color += 60)
		{
			for (int back = 44; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);
				printf("\n");
			}
		}
	}
	return 0;
}