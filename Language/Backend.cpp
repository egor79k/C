#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Asm_commands.h"
#include "Tree.h"
#include "DSL/DSL_definitions.h"
#include "Common.cpp"

#define BKND_ERROR "\e[1;35mBackend: \e[31merror:\e[0m"


const char IF_LABELS_NAME[]    = ".if_";
const char ELSE_LABELS_NAME[]  = ".else_";
const char WHILE_LABELS_NAME[] = ".while_";
const char STOP_LABELS_NAME[]  = ".stop_";

int If_label_num    = 0;
int While_label_num = 0;
int Sqrt_label_num  = 0;


const int NOT_INITIALAISED_VAR   = -666;// very bad
const int MAX_VARIABLES_IN_BLOCK = 16;	// num of locals for one function
const int VAR_SIZE               = 8;	// bytes

int Global = 123456789;


void BackEnd (tree *root, const char file_name[MAX_FILE_NAME]);
void WriteE (tree *node, FILE *output);
void WriteAsm (tree *node, FILE *output);



int main (int argc, const char **argv)
{
	if (argc < 3)
	{
		printf("%s no input or/and output file\n", BKND_ERROR);
		return 1;
	}
	//printf("\e[1;35mBackend\e[0m called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);

	tree *root = ReadTree (argv[1]);
	if (root == NULL)
	{
		printf("%s Wrong input file.\n", BKND_ERROR);
	}

	BackEnd (root, argv[2]);

	return 0;
}



//===============================
// BackEnd
//===============================
void BackEnd (tree *root, const char file_name[MAX_FILE_NAME])
{
	FILE *output = fopen (file_name, "w");
	fprintf(output, "%s\n", ASM_BEGIN);
	WriteAsm (root, output);
	fclose (output);
	return;
}



//===============================
// Declaring global variables
//===============================
tree *WriteGlobal (tree *node, FILE *output)
{
	if (node->right->data == VAR_NUM)
	{
		int tmp = (int) FindVar (node->right->right->data);
		if (tmp < Global) Global = tmp;
		WriteAsm (node->right, output);
	}
	else return node;
	if (node->left != NULL) return WriteGlobal (node->left, output);
	else return NULL;
}



//===============================
// Interpritation of block parts
//===============================
void WriteAsm (tree *node, FILE *output)
{
	int tmp = 0;
	switch (node->type)
	{
		case FUNCTION:
		{
			switch ((int) node->data)
			{
				case P_NUM:
					if (node->right != NULL) node = WriteGlobal (node->right, output);
					fprintf(output, "\tadd rbp, %d\n\tcall Principalis\n%s", MAX_VARIABLES_IN_BLOCK * VAR_SIZE, ASM_END);
					if (node != NULL) WriteAsm (node, output);
					break;


				case B_NUM:
					WriteAsm (node->right, output);
					break;


				case RETURN_NUM:
					if (node->right != NULL)
					{
						WriteE (node->right, output);
						fprintf(output, "%s", ASM_RET_VAL);
					}
					fprintf(output, "\tsub rbp, %d\n\tret\n\n", MAX_VARIABLES_IN_BLOCK * VAR_SIZE);
					break;


				case PRINT_NUM:
					WriteE (node->right, output);
					fprintf (output, ASM_OUT);
					break;


				case READ_NUM:
					tmp = (int) FindVar (node->right->data);
					if (tmp < Global) fprintf (output, "\n\tmov rsi, loc_mem + %d\n\tadd rsi, rbp\n", tmp * VAR_SIZE);
					else fprintf (output, "\n\tmov rsi, loc_mem + %d\n", tmp * VAR_SIZE);
					fprintf (output, ASM_IN);
					break;


				default:
					printf("%s unknown function %g\n", BKND_ERROR, node->data);
					break;
			}
			break;
		}

		case OPERATOR:
		{
			switch ((int) node->data)
			{
				case D_NUM:
					WriteAsm (node->right, output);
					if (node->left != NULL) WriteAsm (node->left, output);
					break;


				case VAR_NUM:
					if (node->parent->data != VARLIST_NUM)
					{
						if (node->left != NULL) WriteE (node->left, output);
						else fprintf (output, "\t%s %d\n", ASM_PUSH, NOT_INITIALAISED_VAR);
					}
					fprintf (output, ASM_LOC_VAR, ASM_POP, (int) FindVar (node->right->data) * VAR_SIZE);
					break;


				case DEF_NUM:
					fprintf (output, "\n\n%s:\n", functions[FindFunc (node->right->data)].name);
					if (node->left != NULL)
					{
						fprintf(output, "\tpop rax     ; Save ret addr\n");
						WriteAsm (node->left, output);
						fprintf(output, "\tpush rax    ; Push ret addr\n");
					}
					WriteAsm (node->right->right, output);
					break;


				case VARLIST_NUM:
					if (node->right->data == VAR_NUM) WriteAsm (node->right, output);
					else WriteE (node->right, output);
					if (node->left != NULL) WriteAsm (node->left, output);
					break;


				case OPER_NUM:
					WriteAsm (node->right, output);
					if (node->left != NULL) WriteAsm (node->left, output);
					break;


				case ASSN_NUM:
					WriteE (node->right, output);
					tmp = (int) FindVar (node->left->data);
					if (tmp < Global) fprintf (output, ASM_LOC_VAR, ASM_POP, tmp * VAR_SIZE);
					else fprintf (output, ASM_GLOB_VAR, ASM_POP, tmp * VAR_SIZE);
					break;


				case CALL_NUM:
					if (node->left != NULL) WriteAsm (node->left, output);
					fprintf (output, "\tadd rbp, %d\n", MAX_VARIABLES_IN_BLOCK * VAR_SIZE);
					fprintf (output, "\tcall %s\n", functions[FindFunc (node->right->data)].name);
					break;


				case IF_NUM:
				{
					int Cur_label = If_label_num++;
					WriteE (node->left->right, output);
					WriteE (node->left->left, output);
					fprintf(output, ASM_CMP);

					switch ((int) node->left->data)
					{
						#define JMP(asm, num, l_num) case num: fprintf (output, #asm); break;
						#include "DSL/DSL_jumps.h"
						#undef JMP
					}

					fprintf (output, " %s%d\n", IF_LABELS_NAME, Cur_label);
					WriteAsm (node->right->right, output);
					if (node->right->left != NULL) fprintf (output, "\t%s %s%d\n", ASM_JMP, ELSE_LABELS_NAME, Cur_label);
					fprintf (output, "%s%d:\n", IF_LABELS_NAME, Cur_label);
					if (node->right->left != NULL)
					{
						WriteAsm (node->right->left, output);
						fprintf (output, "%s%d:\n", ELSE_LABELS_NAME, Cur_label);
					}			
					break;
				}


				case WHILE_NUM:
					fprintf(output, "%s%d:\n", WHILE_LABELS_NAME, While_label_num);
					WriteE (node->left->right, output);
					WriteE (node->left->left, output);
					fprintf(output, ASM_CMP);

					switch ((int) node->left->data)
					{
						#define JMP(asm, num, l_num) case num: fprintf (output, #asm); break;
						#include "DSL/DSL_jumps.h"
						#undef JMP
					}

					fprintf (output, " %s%d\n", STOP_LABELS_NAME, While_label_num);
					WriteAsm (node->right, output);
					fprintf(output, "\t%s %s%d\n%s%d:\n", ASM_JMP, WHILE_LABELS_NAME, While_label_num++, STOP_LABELS_NAME, While_label_num);
					break;


				default:
					printf("%s unknown operator %g\n", BKND_ERROR, node->data);
					break;
			}
			break;
		}

		case VARIABLE:
			printf("%s expected function or operator, recieved variable %g\n", BKND_ERROR, node->data);
			break;

		case CONSTANT:
			printf("%s expected function or operator, recieved constant %g\n", BKND_ERROR, node->data);
			break;
	}
	return;
}



//===============================
// Interpritation of expression
//===============================
void WriteE (tree *node, FILE *output)
{
	int tmp = 0;
	switch (node->type)
	{
		case FUNCTION:
			WriteE (node->right, output);
			if ((int) node->data == SQRT_NUM)
			{
					fprintf(output, ASM_SQRT, Sqrt_label_num, Sqrt_label_num, Sqrt_label_num, Sqrt_label_num);
					Sqrt_label_num++;
			}
			break;


		case OPERATOR:
			if (node->left  != NULL && node->data != CALL_NUM) WriteE (node->left, output);
			if (node->right != NULL && node->data != CALL_NUM) WriteE (node->right, output);

			switch ((int) node->data)
			{
				case PLUS_NUM:
					fprintf (output, ASM_ADD);
					break;

				case MINUS_NUM:
					fprintf (output, ASM_SUB);
					break;

				case MUL_NUM:
					fprintf (output, ASM_MUL);
					break;

				case DIV_NUM:
					fprintf (output, ASM_DIV);
					break;

				case CALL_NUM:
					WriteAsm (node, output);
					break;
			}
			break;


		case VARIABLE:
			tmp = (int) FindVar (node->data);
			if (tmp < Global) fprintf (output, ASM_LOC_VAR, ASM_PUSH, tmp * VAR_SIZE);
			else fprintf (output, ASM_GLOB_VAR, ASM_PUSH, tmp * VAR_SIZE);
			break;


		case CONSTANT:
			fprintf(output, "\t%s %d\n", ASM_PUSH, (int) node->data);
			break;
	}
	return;
}