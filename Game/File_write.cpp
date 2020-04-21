#include <iostream>

unsigned short SCREEN_LENGTH = 169;	// Screen size
unsigned short SCREEN_HEIGHT = 42;

int main ()
{
	unsigned char init_size = 6 * sizeof (unsigned short) + 20;
	unsigned char initial[init_size] = {"AAAAAAAAAAAAWorld_1\0"};

	unsigned char buffer[SCREEN_HEIGHT * 2 + 2][SCREEN_LENGTH * 3 + 2] = {};
	int i = 0;

	for (i = 0; i < SCREEN_LENGTH * 3 + 2; ++i)
	{
		buffer[0][i] = 1;
		buffer[SCREEN_HEIGHT * 2 - 2][i] = 3;
		buffer[SCREEN_HEIGHT * 2 - 1][i] = 2;
		buffer[SCREEN_HEIGHT * 2][i]     = 2;
		buffer[SCREEN_HEIGHT * 2 + 1][i] = 1;
	}
	for (i = 0; i < SCREEN_HEIGHT * 2 + 2; ++i)
	{
		buffer[i][0] = 1;
		buffer[i][SCREEN_LENGTH * 3 + 1] = 1;
	}

	for (int y = 0; y < SCREEN_HEIGHT * 2 + 2; ++y)
	{
		for (int x = 0; x < SCREEN_LENGTH * 3 + 2; ++x)
		{
			if (buffer[y][x]) putchar ('1');
			else putchar ('0');
		}
		putchar ('\n');
	}

	FILE *output = fopen ("new_world.g", "wb");
	fwrite (initial, sizeof (unsigned char), init_size, output);
	fwrite (buffer, sizeof (unsigned char), (SCREEN_HEIGHT * 2 + 2) * (SCREEN_LENGTH * 3 + 2), output);
	fclose (output);
	return 0;
}