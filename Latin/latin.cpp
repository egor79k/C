//::::::::::::::::::::::::::::::::::::::::::::::::::::
//call format: "./latin [file.latin] [flags] [args]"
//
//flags:	-o [output_file]	-output file naming
//			-p 					-only preprocessing
//			-t 					-only preprocessing and translation
//			-n
//			-l
//			-f 					-save all intermediary files
//			-c 					-quote M.Cicero
//::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR "\e[1;31merror:\e[0m"

//::::::::::::::::::::
// Preprocessor <=> P
// Translator   <=> T
// NASM         <=> N
// LD           <=> L
//::::::::::::::::::::

const int MAX_FILE_NAME_LENGHT = 50;

const char *P_input_file_type = ".latin";
const char *P_T_file_type     = ".lp";
const char *T_N_file_type     = ".asm";
const char *L_input_file_type = ".o";
const char *L_name_flag       = " -o ";

bool SAVE_INTER_FILES = false;
bool ONLY_P           = false;
bool ONLY_P_T         = false;



int main (int argc, char ** argv)
{
	if (argc < 2)
	{
		printf("%s no arguments\n", ERROR);
		return 1;
	}

	char make_dir   [MAX_FILE_NAME_LENGHT]      = "mkdir";
	char del_dir    [MAX_FILE_NAME_LENGHT]      = "rm -r";
	char P_call [3 * MAX_FILE_NAME_LENGHT + 15] = "./Preprocessor ";
	char T_call [4 * MAX_FILE_NAME_LENGHT + 13] = "./Translator ";
	char N_call [2 * MAX_FILE_NAME_LENGHT + 14] = "nasm -f elf64 ";
	char L_call [3 * MAX_FILE_NAME_LENGHT + 3]  = "ld ";
	char folder_name[MAX_FILE_NAME_LENGHT]      = " ";

	char *output_file = 0;
	int file_name_index = 0;
	int arg_num = 1;
	int i = 0;

	while (argv[arg_num][i] != '.' && argv[arg_num][i] != '\0')
	{
		if (argv[arg_num][i] == '/') file_name_index = i + 1;
		i++;
	}

	if (strcmp (&argv[arg_num][i], P_input_file_type))
	{
		printf("%s wrong type of input file type: \"\e[31m%s\e[0m\"\n", ERROR, argv[arg_num]);
		return 1;
	}

	strcpy (&P_call[15], argv[arg_num]);

	argv[arg_num][i] = '/';
	argv[arg_num][i + 1] = '\0';
	strcpy (&folder_name[1], argv[arg_num]);
	strcpy (&make_dir[5], folder_name);
	system (make_dir);
	strcpy (&del_dir[5], folder_name);

	strcpy (&argv[arg_num][i], P_T_file_type);
	strcpy (&P_call[strlen (P_call)], folder_name);
	strcpy (&P_call[strlen (P_call)], &argv[arg_num][file_name_index]);

	strcpy (&T_call[13], folder_name);
	strcpy (&T_call[strlen (T_call)], &argv[arg_num][file_name_index]);

	strcpy (&argv[arg_num][i], T_N_file_type);
	strcpy (&T_call[strlen (T_call)], folder_name);
	strcpy (&T_call[strlen (T_call)], &argv[arg_num][file_name_index]);

	argv[arg_num][i] = '\0';
	output_file = &argv[arg_num][file_name_index];
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

				case 'f':
					SAVE_INTER_FILES = true;
					break;

				case 'p':
					ONLY_P = true;
					break;

				case 't':
					ONLY_P_T = true;
					break;

				default:
					printf("%s unlnown flag: \"\e[31m%s\e[0m\"\n", ERROR, argv[arg_num]);
					return 1;
					break;
			}
		}
		else
		{
			printf("%s expected flag '\e[1;32m-_\e[0m' here: \"\e[31m%s\e[0m\"\n", ERROR, argv[arg_num]);
			return 1;
		}
		arg_num++;
	}


	system (P_call);
	if (!ONLY_P)
	{
		system (T_call);
		if (!ONLY_P_T)
		{
			system (N_call);
			system (L_call);
		}
	}
	//strcpy (output_file, argv[arg_num]);

	printf("%s\n%s\n%s\n", P_call, T_call, output_file);

	if (!SAVE_INTER_FILES) system (del_dir);

	return 0;
}