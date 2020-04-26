#include <iostream>
#include <curses.h>
#include "kbhit.cpp"
#include "Blocks.h"
#include "DSL.h"
#include "Variables.h"

void Initialize_screen ();
void Save_world ();
void Open_world ();


int main ()
{
	initscr ();
	keypad (stdscr, 1);
	refresh ();

	Open_world ();

	puts ("\e[?25l\e[90;104m\e[2J");

	Initialize_screen ();

	int ch = 0;
	char player = '^';
	SetPlayer

	while (true)
	{
		COUNTER++;
		ON_DEBUG_PROF (printf ("\e[4;1HTime: %llu", COUNTER));

		if (_kbhit ())
		{
			ch = getch();
			switch (ch)
			{
				case 'q':
					endwin();
					puts ("\e[0m\e[2J\e[?25h");
					return 0;
					break;

				case KEY_UP:
					player = '^';
					if (!MATRIX[GLOBAL_PY - 1][GLOBAL_PX])
					{
						RemovePlayer
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
					SetPlayer
					break;
				
				case KEY_DOWN:
					player = 'v';
					if (!MATRIX[GLOBAL_PY + 1][GLOBAL_PX])
					{
						RemovePlayer
						GLOBAL_PY++;
						SCREEN_PY = GLOBAL_PY % LINES;
						if (SCREEN_PY == 0) SCREEN_PY = LINES;
						if (SCREEN_PY == 1)
						{
								CHUNK_Y++;
								Initialize_screen ();
						}
					}
					SetPlayer
					break;

				case KEY_LEFT:
					player = '<';
					if (!MATRIX[GLOBAL_PY][GLOBAL_PX - 1])
					{
						RemovePlayer
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
					SetPlayer
					break;

				case KEY_RIGHT:
					player = '>';
					if (!MATRIX[GLOBAL_PY][GLOBAL_PX + 1])
					{
						RemovePlayer
						GLOBAL_PX++;
						SCREEN_PX = GLOBAL_PX % COLS;
						if (SCREEN_PX == 0) SCREEN_PX = COLS;
						if (SCREEN_PX == 1)
						{
								CHUNK_X++;
								Initialize_screen ();
						}
					}
					SetPlayer
					break;
			}

			ON_DEBUG(printf("\e[2;1HPlayer: global: %d %d; screen: %d %d\e[3;1HChunks: %d %d", GLOBAL_PX, GLOBAL_PY, SCREEN_PX, SCREEN_PY, CHUNK_X, CHUNK_Y);)
		}
	}

	Save_world ();
	endwin();
	puts ("\e[0m\e[2J\e[?25h");
	return 0;
}

ON_DEBUG (int count = 0;)
void Initialize_screen ()
{
	printf ("\e[1;1H");
	ON_DEBUG(count++;)
	for (int y = LINES * CHUNK_Y + 1; y < LINES * (CHUNK_Y + 1) + 1 && y < GLOBAL_HEIGHT; ++y)
	{
		for (int x = COLS * CHUNK_X + 1; x < COLS * (CHUNK_X + 1) + 1 && x < GLOBAL_LENGTH; ++x)
		{
			printf("%s", BLOCKS[MATRIX[y][x]]);
		}
		printf("\e[E");
	}
	printf ("\e[90;104m");
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

	FILE *output = fopen (WORLD_FILE_NAME, "wb");

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

	FILE *input = fopen (WORLD_FILE_NAME, "rb");

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

	//printf("V: %d, L: %d, H: %d, G: %d, X: %d, Y: %d, %s\n", VERSION, GLOBAL_LENGTH, GLOBAL_HEIGHT, HORIZONT, GLOBAL_PX, GLOBAL_PY, WORLD_NAME);

	free (init_start);

	MATRIX = (uc_t **) calloc (GLOBAL_HEIGHT, sizeof (uc_t*));
	for (int i = 0; i < GLOBAL_HEIGHT; ++i)
		MATRIX[i] = (uc_t *) calloc (GLOBAL_LENGTH, sizeof (uc_t));

	for (int i = 0; i < GLOBAL_HEIGHT; ++i) fread (MATRIX[i], sizeof (uc_t), GLOBAL_LENGTH, input);

	while (GLOBAL_PY > LINES  * (CHUNK_Y + 1)) CHUNK_Y++;
	while (GLOBAL_PX > COLS * (CHUNK_X + 1)) CHUNK_X++;

	SCREEN_PX = GLOBAL_PX % COLS;
	SCREEN_PY = GLOBAL_PY % LINES;

	return;
}
#undef INITIAL_READ