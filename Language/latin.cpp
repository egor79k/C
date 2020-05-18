//::::::::::::::::::::::::::::::::::::::::::::::::::::
//call format: "./latin [file.latin] [flags] [args]"
//
//flags:	
//			-h 					-help
//			-o [output_file]	-output file naming
//			-f 					-only frontending
//			-b 					-only frontending and backending
//			-n 					-only frontending, backending and nasm
//			-t 					-draw frontend tree
//			-s 					-save all intermediary files
//			-c 					-quote M.Cicero
//::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR "\e[1;31merror:\e[0m"

//::::::::::::::::::::
// Frontend     <=> F
// Backend      <=> B
// NASM         <=> N
// LD           <=> L
//::::::::::::::::::::

const int MAX_FILE_NAME_LENGHT = 50;

const char *F_input_file_type = ".latin";
const char *F_B_file_type     = ".txt";
const char *B_N_file_type     = ".asm";
const char *Tree_file_type     = ".svg";
const char *L_input_file_type = ".o";
const char *L_name_flag       = " -o ";
const char *HELP_FLAG         = "-h";
const char *FOLDER_POSTFIX    = "_temp/";

bool SAVE_INTER_FILES = false;
bool ONLY_F           = false;
bool ONLY_F_B         = false;
bool ONLY_F_B_N       = false;
bool DRAW_TREE        = false;


void Help ()
{
	printf("\n\e[1;32m::::flags::::\e[0m\n");
	printf ("-h  help\n");
	printf ("-o  [output_file]  output file naming\n");
	printf ("-f  only frontending\n");
	printf ("-b  only frontending and backending\n");
	printf ("-n  only frontending, backending and nasm\n");
	printf ("-t  draw frontend tree\n");
	printf ("-s  save all intermediary files\n");
	printf ("-c  quote M.Cicero (NOT SUPPORTED YET)\n\n");
	return;
}



int main (int argc, char ** argv)
{
	if (argc < 2)
	{
		printf("%s no arguments. Call \"\e[1;32m./latin -h\e[0m\" for help.\n", ERROR);
		return 1;
	}

	if (!strcmp (argv[1], HELP_FLAG))
	{
		Help ();
		return 1;
	}

	char make_dir   [MAX_FILE_NAME_LENGHT]      = "mkdir";
	char del_dir    [MAX_FILE_NAME_LENGHT]      = "rm -r";
	char F_call [3 * MAX_FILE_NAME_LENGHT + 11] = "./Frontend ";
	char B_call [4 * MAX_FILE_NAME_LENGHT + 10] = "./Backend ";
	char N_call [2 * MAX_FILE_NAME_LENGHT + 14] = "nasm -f elf64 ";
	char L_call [3 * MAX_FILE_NAME_LENGHT + 3]  = "ld ";
	char folder_name[MAX_FILE_NAME_LENGHT]      = " ";

	char *output_file = 0;
	int file_name_index = 0;
	int arg_num = 1;
	int type_index = 0;

	while (argv[arg_num][type_index] != '.' && argv[arg_num][type_index] != '\0')
	{
		if (argv[arg_num][type_index] == '/') file_name_index = type_index + 1;
		type_index++;
	}

	if (strcmp (&argv[arg_num][type_index], F_input_file_type))
	{
		printf("%s wrong type of input file type: \"\e[31m%s\e[0m\"\n", ERROR, argv[arg_num]);
		return 1;
	}

	strcpy (&F_call[15], argv[arg_num]);
	strcpy (&F_call[strlen (F_call)], argv[arg_num]);

	//argv[arg_num][type_index] = '/';
	//argv[arg_num][type_index + 1] = '\0';
	strcpy (&argv[arg_num][type_index], FOLDER_POSTFIX);
	strcpy (&folder_name[1], argv[arg_num]);
	strcpy (&make_dir[5], folder_name);
	system (make_dir);
	strcpy (&del_dir[5], folder_name);

	strcpy (&argv[arg_num][type_index], F_B_file_type);
	strcpy (&F_call[strlen (F_call)], folder_name);
	strcpy (&F_call[strlen (F_call)], &argv[arg_num][file_name_index]);

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

				case 's':
					SAVE_INTER_FILES = true;
					break;

				case 'f':
					ONLY_F = true;
					SAVE_INTER_FILES = true;
					break;

				case 'b':
					ONLY_F_B = true;
					SAVE_INTER_FILES = true;
					break;

				case 'n':
					ONLY_F_B_N = true;
					SAVE_INTER_FILES = true;
					break;

				case 't':
					DRAW_TREE = true;
					strcpy (&argv[1][type_index], Tree_file_type);
					strcpy (&F_call[strlen (F_call)], folder_name);
					strcpy (&F_call[strlen (F_call)], &argv[1][file_name_index]);
					SAVE_INTER_FILES = true;
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



	printf("%s\n", F_call);

	system (F_call);

	if (!ONLY_F)
	{
		if (DRAW_TREE) strcpy (&argv[1][type_index], F_B_file_type);
		strcpy (&B_call[10], folder_name);
		strcpy (&B_call[strlen (B_call)], &argv[1][file_name_index]);

		strcpy (&argv[1][type_index], B_N_file_type);
		strcpy (&B_call[strlen (B_call)], folder_name);
		strcpy (&B_call[strlen (B_call)], &argv[1][file_name_index]);
printf("%s\n", B_call);
		system (B_call);

		if (!ONLY_F_B)
		{
			strcpy (&N_call[14], folder_name);
			strcpy (&N_call[strlen (N_call)], &argv[1][file_name_index]);
printf("%s\n", N_call);
			system (N_call);

			if (!ONLY_F_B_N)
			{
				strcpy (&argv[1][type_index], L_input_file_type);
				strcpy (&L_call[3], folder_name);
				strcpy (&L_call[strlen (L_call)], &argv[1][file_name_index]);
				strcpy (&L_call[strlen (L_call)], L_name_flag);
				argv[1][type_index] = '\0';
				strcpy (&L_call[strlen (L_call)], &argv[1][0]);
printf("%s\n", L_call);
				system (L_call);
			}
		}
	}
	
	if (!SAVE_INTER_FILES) system (del_dir);

	return 0;
}