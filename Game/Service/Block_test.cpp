#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// |Nope|Stone|Dirt|Grass|
// ^    ^     ^    ^     ^
// 0    1     5    7


//Stone:"\e[90m\U00002588"
//		"\e[2;37;40m\U00002592"
//		"\e[2;97;40m\U00002592"
//		"\e[2;37;40m\U00002593"
//		"\e[2;97;40m\U00002593"

//Dirt: "\e[30;43m\U00002593"	// Dark dirt
//		"\e[2;33;40m\U00002592"	// Light dirt
//		"\e[33;40m\U00002591"	// Wet dirt
//		"\e[2;93;40m\U00002591"	// Stony dirt


//Limestone: "\e[37;40m\U00002588"

#define DIRT_D "\e[30;43m\U00002593"
#define DIRT_L "\e[2;33;40m\U00002592"
#define DIRT_W "\e[33;40m\U00002591"
#define DIRT_S "\e[2;93;40m\U00002591"

int main ()
{
	int i = 0;

	puts ("\e[104m\n\n\n\n\n\n\n\n\n\n\n\n\n");

	srand (time (0));

	for (i = 0; i < 1000; ++i)
	{
		switch (random () % 3)
		{
			case 0: printf ("\e[30;43m\U00002593");
			break;

			case 1: printf ("\e[2;33;40m\U00002592");
			break;

			case 2: if (random () % 5 == 0) printf ("\e[2;93;40m\U00002591");
			break;
		}
	}
	printf("\n");

	for (i = 0; i < 2000; ++i)
	{
		switch (random () % 3)
		{
			case 0: if (random () % 5 == 0) printf ("\e[90m\U00002588");
			break;

			case 1: printf ("\e[2;37;40m\U00002592");
			break;

			case 2: printf ("\e[2;97;40m\U00002592");
			break;
		}
	}



	/*printf("\e[90m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002588");

	printf ("\e[37;40m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002591");

	printf ("\e[97;40m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002591");
*/
	printf ("\e[2;33;47m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002592");

	printf ("\e[2;93;47m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002592");

	printf ("\e[2;33;47m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002593");

	printf ("\e[2;93;47m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002593");
/*
	printf ("\e[2;37;40m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002593");

	printf ("\e[2;97;40m");

	for (i = 0; i < 10; ++i)
		printf ("\U00002593");
*/
	puts ("\e[0m");

	return 0;
}