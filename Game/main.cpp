#include <iostream>
#include <curses.h>
#include "Blocks.h"

#define CPrint(X, Y, STR)	printf("\e[%d;%dH%c", Y, X, STR);

typedef unsigned short us_t;
typedef unsigned char uc_t;
typedef unsigned int ui_t;


const int WORLD_NAME_SIZE = 20;
char *WORLD_NAME = 0;

const int INIT_SIZE = 6 * sizeof (us_t) + WORLD_NAME_SIZE;
char *INITIAL = {};

const us_t VERSION = 0;

us_t GLOBAL_LENGTH = 171;	// World size
us_t GLOBAL_HEIGHT = 44;
us_t SCREEN_LENGTH = 169;	// Screen size
us_t SCREEN_HEIGHT = 42;

us_t HORIZONT = 0;

us_t CHUNK_X = 0;
us_t CHUNK_Y = 0;

us_t GLOBAL_PX = 1;				//Player position
us_t GLOBAL_PY = 36;
us_t SCREEN_PX = 1;				//Player position
us_t SCREEN_PY = 36;
					// ^ M[*][]
uc_t **MATRIX = 0;	// |             Gobal world matrix
					// +---> M[][*]


void Open_world ();
void Initialize_screen ();


int main ()
{
	initscr ();
	keypad (stdscr, 1);
	refresh ();

	GLOBAL_LENGTH = COLS;
	GLOBAL_HEIGHT = LINES;

	INITIAL = (char *) calloc (INIT_SIZE, sizeof (char));

	Open_world ();

	while (GLOBAL_PY > LINES  * (CHUNK_Y + 1)) CHUNK_Y++;
	while (GLOBAL_PX > COLS * (CHUNK_X + 1)) CHUNK_X++;

	SCREEN_PX = GLOBAL_PX % COLS;
	SCREEN_PY = GLOBAL_PY % LINES;

	puts ("\e[?25l\e[32;104m\e[2J");

	Initialize_screen ();

	//printf("CX: %d, CY: %d, COLS: %d, LINES: %d\n", CHUNK_X, CHUNK_Y, COLS, LINES);

//getchar ();

	//puts ("\e[?25l\e[32;104m\e[2J\e[35B\e[1;1H");
	//puts ("\e[?25l\e[32;104m\e[2J\e[1;1H");

	int ch = 0;
	char player = '^';

	do
	{
		CPrint (SCREEN_PX, SCREEN_PY, player);
		fflush (stdout);
		ch = getch();
		CPrint (SCREEN_PX, SCREEN_PY, ' ');

		switch (ch)
		{
			case KEY_UP:
				player = '^';
				if (!MATRIX[GLOBAL_PY - 1][GLOBAL_PX])
				{
					GLOBAL_PY--;
					SCREEN_PY = GLOBAL_PY % LINES;
					if (SCREEN_PY == 0)
					{
						if (CHUNK_Y)
						{
							CHUNK_Y--;
							Initialize_screen ();
						}
					}
				}
				break;
			
			case KEY_DOWN:
				player = 'v';
				if (!MATRIX[GLOBAL_PY + 1][GLOBAL_PX])
				{
					GLOBAL_PY++;
					SCREEN_PY = GLOBAL_PY % LINES;
					if (SCREEN_PY == 0)
					{
							CHUNK_Y++;
							Initialize_screen ();
					}
				}
				break;

			case KEY_LEFT:
				player = '<';
				if (!MATRIX[GLOBAL_PY][GLOBAL_PX - 1])
				{
					GLOBAL_PX--;
					SCREEN_PX = GLOBAL_PX % COLS;
					if (SCREEN_PX == 0)
					{
						if (CHUNK_X)
						{
							CHUNK_X--;
							Initialize_screen ();
						}
					}
				}
				break;

			case KEY_RIGHT:
				player = '>';
				if (!MATRIX[GLOBAL_PY][GLOBAL_PX + 1])
				{
					GLOBAL_PX++;
					SCREEN_PX = GLOBAL_PX % COLS;
					if (SCREEN_PX == 0)
					{
							CHUNK_X++;
							Initialize_screen ();
					}
				}
				break;
		}

	} while (ch != 'q');

	endwin();
	puts ("\e[0m\e[2J\e[?25h");
	return 0;
}


void Initialize_screen ()
{
	puts ("\e[1;1H");

	for (int y = LINES * CHUNK_Y + 1; y < LINES * (CHUNK_Y + 1) + 1; ++y)
	{
		for (int x = COLS * CHUNK_X + 1; x < COLS * (CHUNK_X + 1) + 1; ++x)
		{
			printf("%s", BLOCKS[MATRIX[y][x]]);
		}
	}

	return;
}

/*
void Main_menu ()
{
	const char PUNCTS = 3;
	CPrint (COLS / 2 - 5, LINES / 2 - 5, "\e[1;32mMain  menu\e[0m\n");

}
*/


/*void Save_world ()
{
	FILE *output = fopen ("new_world.g", "wb");

	INITIAL

	for (int i = 0; i < )

	
}*/


void Open_world ()
{
	char initial[INIT_SIZE] = {};

	FILE *input = fopen ("new_world.g", "rb");
	//fseek (input, 0, SEEK_END);
	//int NChars = ftell (input);
	//fseek (input, 0, SEEK_SET);

	fread (INITIAL, sizeof (char), INIT_SIZE, input);

	if (*((us_t *) INITIAL) != VERSION) printf("Error: wrong version %d! Must be %d.\n", *((us_t *) INITIAL), VERSION);
	INITIAL += sizeof (us_t);
	
	GLOBAL_LENGTH = *((us_t *) INITIAL);
	INITIAL += sizeof (us_t);
	
	GLOBAL_HEIGHT = *((us_t *) INITIAL);
	INITIAL += sizeof (us_t);

	HORIZONT = *((us_t *) INITIAL);
	INITIAL += sizeof (us_t);
	
	GLOBAL_PX = *((us_t *) INITIAL);
	INITIAL += sizeof (us_t);
	
	GLOBAL_PY = *((us_t *) INITIAL);
	INITIAL += sizeof (us_t);
	
	WORLD_NAME = INITIAL;
	INITIAL += WORLD_NAME_SIZE;

	printf("V: %d, L: %d, H: %d, G: %d, X: %d, Y: %d, %s\n", VERSION, GLOBAL_LENGTH, GLOBAL_HEIGHT, HORIZONT, GLOBAL_PX, GLOBAL_PY, WORLD_NAME);

	MATRIX = (uc_t **) calloc (GLOBAL_HEIGHT, sizeof (uc_t*));
	for (int i = 0; i < GLOBAL_HEIGHT; ++i)
		MATRIX[i] = (uc_t *) calloc (GLOBAL_LENGTH, sizeof (uc_t));

	for (int i = 0; i < GLOBAL_HEIGHT; ++i) fread (MATRIX[i], sizeof (uc_t), GLOBAL_LENGTH, input);

	return;
}