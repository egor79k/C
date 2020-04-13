#include <stdio.h>
#include "Commands.h"
#include "Common.h"

#define IS_IDENTIFICATOR ('a' <= *buffer && *buffer <= 'z') || ('A' <= *buffer && *buffer <= 'Z') || ('0' <= *buffer && *buffer <= '9') || *buffer == '_'
#define TRLR "\e[1;35mTranslator:\e[0m"

char *buffer = 0;
const int xor_hash_val = 1111111111;


int Identificator ()
{
	int hash = 0;

	while (IS_IDENTIFICATOR)
	{
		hash = hash xor xor_hash_val;
		hash += *buffer;
		buffer++;
	}

	return hash & 0xFFF;
}


int main (int argc, char** argv)
{
	if (argc < 3)
	{
		printf("%s %s no input or/and output file\n", TRLR, ERROR);
		return 1;
	}
	printf("Translator called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);
	return 0;


	/*FILE *input = fopen (argv[1], "r");
	fseek (input, 0, SEEK_END);
	int NChars = ftell (input);
	fseek (input, 0, SEEK_SET);
	buffer = (char *) calloc (NChars, sizeof (char));
	fread (buffer, sizeof (char), NChars, input);
	fclose (input);
int val = 0;
	while (*buffer != '\0')
	{
		val = Identificator ();
		buffer++;
		printf("%d ", val);
		switch (val)
		{
			case IF_NUM:
				printf ("%s\n", IF);
				break;

			case ELSE_NUM:
				printf ("%s\n", ELSE);
				break;

			case WHILE_NUM:
				printf ("%s\n", WHILE);
				break;

			case DEF_NUM:
				printf ("%s\n", DEF);
				break;
	
			case VAR_NUM:
				printf ("%s\n", VAR);
				break;

			case RETURN_NUM:
				printf ("%s\n", RETURN);
				break;

			case PRINT_NUM:
				printf ("%s\n", PRINT);
				break;

			case READ_NUM:
				printf ("%s\n", READ);
				break;

			case EQAL_NUM:
				printf ("%s\n", EQAL);
				break;

			case NO_EQAL_NUM:
				printf ("%s\n", NO_EQAL);
				break;

			case ABOVE_NUM:
				printf ("%s\n", ABOVE);
				break;

			case ABOVE_EQ_NUM:
				printf ("%s\n", ABOVE_EQ);
				break;

			case BELOW_NUM:
				printf ("%s\n", BELOW);
				break;

			case BELOW_EQ_NUM:
				printf ("%s\n", BELOW_EQ);
				break;

			case ASSN_NUM:
				printf ("%s\n", ASSN);
				break;

			case PLUS_NUM:
				printf ("%s\n", PLUS);
				break;

			case MINUS_NUM:
				printf ("%s\n", MINUS);
				break;

			case MUL_NUM:
				printf ("%s\n", MUL);
				break;

			case DIV_NUM:
				printf ("%s\n", DIV);
				break;

			case POW_NUM:
				printf ("%s\n", POW);
				break;

			case CLOSE_NUM:
				printf ("%s\n", CLOSE);
				break;
		}
	}
	return 0;*/
}

