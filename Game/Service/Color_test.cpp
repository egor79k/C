#include <stdio.h>

#define ALL_COLORS printf("\e[30m\\e[30m \e[31m\\e[31m \e[32m\\e[32m \e[33m\\e[33m \e[34m\\e[34m \e[35m\\e[35m \e[36m\\e[36m \e[37m\\e[37m |\n");

int main ()
{
	printf("\n\e[1mColor table:\n\n| Style | Color\t\t\t\t\t\t\t  |\n");
	printf("| \e[0m\\e[0m | ");
	ALL_COLORS
	printf("| \e[1m\\e[1m | ");
	ALL_COLORS
	printf("| \e[0;2m\\e[2m | ");
	ALL_COLORS
	printf("\e[0m\n");
	
	for (int color = 0; color <= 109; ++color)
	{
		printf("\e[%dm%d\t\e[0m", color, color);
		if ((color + 1) % 10 == 0) printf("\n");
	}
	return 0;
}