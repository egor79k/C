#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Asm_commands.h"

#define BKND_ERROR "\e[1;35mBackend: \e[31merror:\e[0m"

const char IF_LABELS_NAME[] = "if_";
const char ELSE_LABELS_NAME[] = "else_";
int If_label_num = 0;

const char WHILE_LABELS_NAME[] = "while_";
const char STOP_LABELS_NAME[] = "stop_";
int While_label_num = 0;

const int NOT_INITIALAISED_VAR = -666;
const int MAX_VARIABLES_IN_BLOCK = 16;
int Global = 123456789;


#include "Tree.h"
#include "DSL/DSL_definitions.h"


void BackEnd (tree *root, const char file_name[MAX_FILE_NAME]);
void WriteE (tree *node, FILE *output);
void WriteAsm (tree *node, FILE *output);

#include "Common.cpp"


int main (int argc, const char **argv)
{
	if (argc < 3)
	{
		printf("%s no input or/and output file\n", BKND_ERROR);
		return 1;
	}
	printf("Backend called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);

	printf("\nBackend:\n|Reading...\n");
	tree *root = ReadTree (argv[1]);
	if (root == NULL)
	{
		printf("%s Wrong input file.\n", BKND_ERROR);
	}
	//printf("Dumping...\n");
	//Dump (root, "Program/backend_tree.png");
	//printf("Printing...\n");
	//PrintTree (root, "out.txt");
	printf("|Operating...\n");
	BackEnd (root, argv[2]);
}


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
					fprintf (output, "PUSH AX\nPUSH %d\nADD\nPOP AX\nCALL Principalis\nEND\n", MAX_VARIABLES_IN_BLOCK);
					if (node != NULL) WriteAsm (node, output);
					break;

				case B_NUM:
					WriteAsm (node->right, output);
					break;

				case RETURN_NUM:
					if (node->right != NULL) WriteE (node->right, output);
					fprintf (output, "PUSH AX\nPUSH %d\nSUB\nPOP AX\nRETURN\n", MAX_VARIABLES_IN_BLOCK);
					break;

				case PRINT_NUM:
					WriteE (node->right, output);
					fprintf (output, "OUT\n");
					break;

				case READ_NUM:
					fprintf (output, "IN\n");
					tmp = (int) FindVar (node->right->data);
					if (tmp < Global) fprintf (output, "POP [AX+%d]\n", tmp);
					else fprintf (output, "POP [%d]\n", tmp);
					break;

				default:
					printf("Error: unknown function %g\n", node->data);
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
						else fprintf (output, "PUSH %d\n", NOT_INITIALAISED_VAR);
					}
					fprintf (output, "POP [AX+%d]\n", (int) FindVar (node->right->data));
					break;

				case DEF_NUM:
					fprintf (output, "\n\n:%s\n", functions[FindFunc (node->right->data)].name);
					if (node->left != NULL) WriteAsm (node->left, output);
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
					if (tmp < Global) fprintf (output, "POP [AX+%d]\n", tmp);
					else fprintf (output, "POP [%d]\n", tmp);
					//fprintf (output, "POP [AX+%d]\n", (int) FindVar (node->left->data));
					break;

				case CALL_NUM:
					if (node->left != NULL) WriteAsm (node->left, output);
					fprintf (output, "PUSH AX\nPUSH %d\nADD\nPOP AX\n", MAX_VARIABLES_IN_BLOCK);
					fprintf (output, "CALL %s\n", functions[FindFunc (node->right->data)].name);
					break;

				case IF_NUM:
				{
					int Cur_label = If_label_num++;
					WriteE (node->left->right, output);
					WriteE (node->left->left, output);
					switch ((int) node->left->data)
					{
						#define JMP(asm, num, l_num) case num: fprintf (output, #asm); break;
						#include "DSL/DSL_jumps.h"
						#undef JMP
					}
					fprintf (output, " %s%d\n", IF_LABELS_NAME, Cur_label);
					WriteAsm (node->right->right, output);
					if (node->right->left != NULL) fprintf (output, "PUSH 1\nPUSH 0\nJNE %s%d\n", ELSE_LABELS_NAME, Cur_label);
					fprintf (output, ":%s%d\n", IF_LABELS_NAME, Cur_label);
					if (node->right->left != NULL)
					{
						WriteAsm (node->right->left, output);
						fprintf (output, ":%s%d\n", ELSE_LABELS_NAME, Cur_label);
					}			
					break;
				}

				case WHILE_NUM:
					fprintf(output, ":%s%d\n", WHILE_LABELS_NAME, While_label_num);
					WriteE (node->left->right, output);
					WriteE (node->left->left, output);
					switch ((int) node->left->data)
					{
						#define JMP(asm, num, l_num) case num: fprintf (output, #asm); break;
						#include "DSL/DSL_jumps.h"
						#undef JMP
					}
					fprintf (output, " %s%d\n", STOP_LABELS_NAME, While_label_num);
					WriteAsm (node->right, output);
					fprintf(output, "JMP %s%d\n:%s%d\n", WHILE_LABELS_NAME, While_label_num++, STOP_LABELS_NAME, While_label_num);
					break;

				default:
					printf("Error: unknown operator %g\n", node->data);
					break;
			}
			break;
		}

		case VARIABLE:
			printf("Error: expected function or operator, recieved variable %g\n", node->data);
			break;

		case CONSTANT:
			printf("Error: expected function or operator, recieved constant %g\n", node->data);
			break;
	}
	return;
}


void BackEnd (tree *root, const char file_name[MAX_FILE_NAME])
{
	FILE *output = fopen (file_name, "w");
	//fprintf(output, "\n\nEND_\n");
	WriteAsm (root, output);
	fprintf(output, "\n\nEND_\n");
}


void WriteE (tree *node, FILE *output)
{
	int tmp = 0;
	switch (node->type)
	{
		case FUNCTION:
			WriteE (node->right, output);
			switch ((int) node->data)
			{
				#define FUNCTION(name, diff_description, asm_name) case name##NUM: fprintf(output, "%s\n", asm_name); break;
				#include "DSL/DSL_function_descriptions.h"
				#undef FUNCTION
			}
			break;

		case OPERATOR:
			if (node->left != NULL && node->data != CALL_NUM) WriteE (node->left, output);
			if (node->right != NULL && node->data != CALL_NUM) WriteE (node->right, output);
			switch ((int) node->data)
			{
				case PLUS_NUM:
					fprintf (output, "ADD\n");
					break;

				case MINUS_NUM:
					fprintf (output, "SUB\n");
					break;

				case MUL_NUM:
					fprintf (output, "MUL\n");
					break;

				case DIV_NUM:
					fprintf (output, "DIV\n");
					break;

				case CALL_NUM:
					WriteAsm (node, output);
					break;
			}
			break;

		case VARIABLE:
			tmp = (int) FindVar (node->data);
			if (tmp < Global) fprintf (output, "PUSH [AX+%d]\n", tmp);
			else fprintf (output, "PUSH [%d]\n", tmp);
			break;

		case CONSTANT:
			fprintf(output, "PUSH %d\n", (int) node->data);
			break;
	}
	return;
}