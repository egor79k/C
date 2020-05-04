#include <iostream>
#include "Blocks.h"


void Print_CT (int color_0, int back_0)
{
	for (int type = 0; type < 3; type += 2)
	{
		for (int color = color_0; color < 100; color += 60)
		{
			for (int back = back_0; back < 100; back += 60)
			{
				printf("\t\e[%d;%d;%dm\U00002591\U00002591\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002592\U00002592\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002593\U00002593\e[0m", type, color, back);
				printf("\e[%d;%d;%dm\U00002588\U00002588\e[0m", type, color, back);

				printf(" \\e[%d;%d;%dm\\U00002591", type, color, back);
				printf(" \\e[%d;%d;%dm\\U00002592", type, color, back);
				printf(" \\e[%d;%d;%dm\\U00002593", type, color, back);
				printf(" \\e[%d;%d;%dm\\U00002588", type, color, back);
			}
			putchar ('\n');
		}
	}
}


int main ()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (j != i)
			{
				printf(" \e[%dm\U00002588\e[0m + \e[%dm \e[0m\n", 30 + i, 40 + j);
				Print_CT (30 + i, 40 + j);
				printf(" \e[%dm\U00002588\e[0m + \e[%dm \e[0m\n", 30 + j, 40 + i);
				Print_CT (30 + j, 40 + i);
			}
		}
	}

	return 0;
}