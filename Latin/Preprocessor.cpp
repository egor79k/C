#include <stdio.h>
#include <stdlib.h>
#include "Common.h"

#define PREP "\e[1;35mPreprocessor:\e[0m"

#define IS_SPASE(x) (x == ' ' || x == '\t')


int main (int argc, char **argv)
{
	if (argc < 3)
	{
		printf ("%s %s no input or/and output file\n", PREP, ERROR);
		return 1;
	}
	printf ("Preprocessor called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);

	char *input = GetBuffer (argv[1]);
	char *input_start = input;
	char *output = (char *) calloc (NChars, sizeof (char));
	char *output_start = output;
	bool was_space = false;
	bool was_NL = true;

	while (*input != '\0')
	{

		while (IS_SPASE(*input))
		{
			if (!was_NL) was_space = true;
			input++;
		}
		
		if (*input == '\n') was_NL = true;
		else was_NL = false;

		if (was_space)
		{
			*output = ' ';
			output++;
			was_space = false;
		}

		*(output++) = *(input++);
	}

	FILE *out = fopen (argv[2], "w");
	fwrite (output_start, sizeof (char), output - output_start, out);
	fclose (out);
	free (input_start);
	free (output_start);

	return 0;
}