#include <iostream>
#include <curses.h>
#include "kbhit.cpp"
#include "Blocks.h"

#define CPrint(X, Y, STR)	printf("\e[%d;%dH%c", Y, X, STR);

#define DEBUG 0

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif

typedef unsigned short us_t;
typedef unsigned char uc_t;
typedef unsigned int ui_t;

const int MAX_FILE_NAME = 50;
char world_file_name[MAX_FILE_NAME] = "new_world.g";

const int WORLD_NAME_SIZE = 20;
char *WORLD_NAME = 0;

const int INIT_SIZE = 6 * sizeof (us_t) + WORLD_NAME_SIZE;
//char *INITIAL = {};

const us_t VERSION = 0;

us_t GLOBAL_LENGTH = 171;	// World size
us_t GLOBAL_HEIGHT = 44;
us_t SCREEN_LENGTH = COLS;	// Screen size
us_t SCREEN_HEIGHT = LINES;

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


void Initialize_screen ();
void Save_world ();
void Open_world ();


int main ()
{
	initscr ();
	keypad (stdscr, 1);
	refresh ();

	//GLOBAL_LENGTH = COLS;
	//GLOBAL_HEIGHT = LINES;
	//SCREEN_LENGTH = COLS;
	//SCREEN_HEIGHT = LINES;

	//INITIAL = (char *) calloc (INIT_SIZE, sizeof (char));

	Open_world ();

	while (GLOBAL_PY > LINES  * (CHUNK_Y + 1)) CHUNK_Y++;
	while (GLOBAL_PX > COLS * (CHUNK_X + 1)) CHUNK_X++;

	SCREEN_PX = GLOBAL_PX % COLS;
	SCREEN_PY = GLOBAL_PY % LINES;

	puts ("\e[?25l\e[90;104m\e[2J");

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
		ON_DEBUG(printf("\e[2;1HPlayer: global: %d %d; screen: %d %d\e[3;1HChunks: %d %d", GLOBAL_PX, GLOBAL_PY, SCREEN_PX, SCREEN_PY, CHUNK_X, CHUNK_Y);)

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
							SCREEN_PY = LINES;
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
					if (SCREEN_PY == 0) SCREEN_PY = LINES;
					if (SCREEN_PY == 1)
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
							SCREEN_PX = COLS;
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
					if (SCREEN_PX == 0) SCREEN_PX = COLS;
					if (SCREEN_PX == 1)
					{
							CHUNK_X++;
							Initialize_screen ();
					}
				}
				break;
		}

	} while (ch != 'q');

	Save_world ();
	endwin();
	puts ("\e[0m\e[2J\e[?25h");
	return 0;
}

int count = 0;
void Initialize_screen ()
{
	puts ("\e[1;1H");
count++;
	for (int y = LINES * CHUNK_Y + 1; y < LINES * (CHUNK_Y + 1) + 1; ++y)
	{
		for (int x = COLS * CHUNK_X + 1; x < COLS * (CHUNK_X + 1) + 1; ++x)
		{
			printf("%s", BLOCKS[MATRIX[y][x]]);
		}
	}

	ON_DEBUG(printf ("\e[1;1HInitialize_screen: (%d - %d)x(%d - %d); %d times", LINES * CHUNK_Y + 1, LINES * (CHUNK_Y + 1) + 1, COLS * CHUNK_X + 1, COLS * (CHUNK_X + 1) + 1, count);)

	return;
}

/*
void Main_menu ()
{
	const char PUNCTS = 3;
	CPrint (COLS / 2 - 5, LINES / 2 - 5, "\e[1;32mMain  menu\e[0m\n");

}
*/

#define INITIAL_WRITE(var)	*((us_t *) initial) = var;	\
							initial += sizeof (us_t);
void Save_world ()
{
	char *initial = (char *) calloc (INIT_SIZE, sizeof (char));
	char *init_start = initial;

	INITIAL_WRITE(VERSION)
	INITIAL_WRITE(GLOBAL_LENGTH)
	INITIAL_WRITE(GLOBAL_HEIGHT)
	INITIAL_WRITE(HORIZONT)
	INITIAL_WRITE(GLOBAL_PX)
	INITIAL_WRITE(GLOBAL_PY)
	
	initial = WORLD_NAME;
	initial += WORLD_NAME_SIZE;

	FILE *output = fopen (world_file_name, "wb");

	fwrite (init_start, sizeof (char), INIT_SIZE, output);
	for (int i = 0; i < GLOBAL_HEIGHT; ++i) fwrite (MATRIX[i], sizeof (uc_t), GLOBAL_LENGTH, output);
	//fwrite (MATRIX, sizeof (uc_t), GLOBAL_HEIGHT * GLOBAL_LENGTH, output);
	fclose (output);
	
	free (init_start);
	return;
}
#undef INITIAL_WRITE



#define INITIAL_READ(var)	var = *((us_t *) initial);	\
							initial += sizeof (us_t);
void Open_world ()
{
	char *initial = (char *) calloc (INIT_SIZE, sizeof (char));
	char *init_start = initial;

	FILE *input = fopen (world_file_name, "rb");

	fread (initial, sizeof (char), INIT_SIZE, input);

	if (*((us_t *) initial) != VERSION) printf("Error: wrong version %d! Must be %d.\n", *((us_t *) initial), VERSION);
	initial += sizeof (us_t);
	
	INITIAL_READ(GLOBAL_LENGTH)
	INITIAL_READ(GLOBAL_HEIGHT)
	INITIAL_READ(HORIZONT)
	INITIAL_READ(GLOBAL_PX)
	INITIAL_READ(GLOBAL_PY)

	WORLD_NAME = initial;
	initial += WORLD_NAME_SIZE;

	printf("V: %d, L: %d, H: %d, G: %d, X: %d, Y: %d, %s\n", VERSION, GLOBAL_LENGTH, GLOBAL_HEIGHT, HORIZONT, GLOBAL_PX, GLOBAL_PY, WORLD_NAME);

	free (init_start);

	MATRIX = (uc_t **) calloc (GLOBAL_HEIGHT, sizeof (uc_t*));
	for (int i = 0; i < GLOBAL_HEIGHT; ++i)
		MATRIX[i] = (uc_t *) calloc (GLOBAL_LENGTH, sizeof (uc_t));

	for (int i = 0; i < GLOBAL_HEIGHT; ++i) fread (MATRIX[i], sizeof (uc_t), GLOBAL_LENGTH, input);

	return;
}
#undef INITIAL_READ