#include <stdio.h>
#include "Common.h"

#define PREP "\e[1;35mPreprocessor:\e[0m"


int main (int argc, char **argv)
{
	if (argc < 3)
	{
		printf("%s %s no input or/and output file\n", PREP, ERROR);
		return 1;
	}
	printf("Preprocessor called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);
	return 0;
}