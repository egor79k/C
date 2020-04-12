//::::::::::::::::::::::::::::::::::::::::::::::::::::
//call format: "./latin [file.latin] [flags] [args]"
//
//flags:	-o [output_file]	-output file naming
//			-p 					-only preprocessing
//			-t 					-only preprocessing and translation
//			-n
//			-l
//			-f 					-save all intermediary files
//::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR "\x1b[1;31merror:\x1b[0m"

const char *preprocessor_file_type = ".lp";


int main (int argc, char ** argv)
{
	if (argc < 2)
	{
		printf("%s no arguments\n", ERROR);
		return 1;
	}
	char preprocessor_call[50] = "./Preprocessor ";
	char translator_call[50] = "./Translator ";
	char *output_file = 0;
	int arg_num = 1;
	int i = 0;

	strcpy (&preprocessor_call[15], argv[arg_num]);
	while (argv[arg_num][i] != '.') i++;
	strcpy (&argv[arg_num][i], preprocessor_file_type);
	strcpy (&translator_call[13], argv[arg_num]);
	argv[arg_num][i] = '\0';
	output_file = argv[arg_num];
	arg_num++;

	while (arg_num < argc)
	{
		if (argv[arg_num][0] == '-')
		{
			switch (argv[arg_num][1])
			{
				case 'o':
					if (arg_num < argc - 1)
					{
						arg_num++;
						output_file = argv[arg_num];
					}
					else
					{
						printf("%s expected output file name after '-o' flag\n", ERROR);
						return 1;
					}
					break;

				default:
					printf("%s unlnown flag: \"\x1b[31m%s\x1b[0m\"\n", ERROR, argv[arg_num]);
					return 1;
					break;
			}
		}
		else
		{
			printf("%s expected flag '\x1b[1;32m-_\x1b[0m' here: \"\x1b[31m%s\x1b[0m\"\n", ERROR, argv[arg_num]);
			return 1;
		}
		arg_num++;
	}
	//strcpy (output_file, argv[arg_num]);

	printf("%s\n%s\n%s\n", preprocessor_call, translator_call, output_file);


	return 0;
}