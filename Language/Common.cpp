const int MAX_VAR_NUM = 20;
const int MAX_FUNC_NUM = 20;
const int MAX_VAR_NAME_LENGTH = 16;
const int MAX_FUNC_HASH_MULTIPLIER = 32768; //= 2^15   16 symbols max

char *str = 0;

double ReadId (const char mode = 'N');


struct label
{
	char name[MAX_VAR_NAME_LENGTH];
	int hash;
};

label variables[MAX_VAR_NUM] = {};
int free_var = 0;

label functions[MAX_FUNC_NUM] = {};
int free_func = 0;


char *GetBuffer (const char file_name[MAX_FILE_NAME])
{
	FILE *in = fopen (file_name, "r");
	if (in == NULL) return NULL;
	fseek (in, 0, SEEK_END);
	int NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	return text;
}


int FindVar (int hash)
{
	for (int i = 0; i < free_var; i++)
	{
		if (variables[i].hash == hash) return i;
	}
	return -1;
}


int FindFunc (int hash)
{
	for (int i = 0; i < free_func; i++)
	{
		if (functions[i].hash == hash) return i;
	}
	return -1;
}


void WriteLabels (tree *node, FILE *out)
{
	if (node->left != NULL) WriteLabels (node->left, out);

	fprintf (out, "\"%p\" [label = \" {", node);

	switch (node->type)
	{
		case CONSTANT:
			fprintf (out, "%g", node->data);
			break;

		case VARIABLE:
		{
			fprintf (out, "%s", variables[FindVar (node->data)].name);
			break;
		}

		default:
			switch ((int) node->data)
			{
				#define FUNCTION(name, diff_description, print_description) case name##NUM: fprintf(out, "%s", name); break;
				#define OPERATOR(name, diff_description, simp_description, print_description) case name##NUM: fprintf(out, "%s%s", #print_description, name); break;
				#include "DSL/DSL_function_descriptions.h"
				#include "DSL/DSL_operator_descriptions.h"
				#undef FUNCTION
				#undef OPERATOR
				default:
					fprintf (out, "%s", functions[FindFunc (node->data)].name);
					break;
			}
			break;
	}

	fprintf(out, " | {left\\n%p | parent\\n%p | level\\n%d | type\\n%c | adress\\n%p | right\\n%p}} \"]\n", node->left, node->parent, node->level, node->type, node, node->right);

	if (node->right != NULL) WriteLabels (node->right, out);
	return;
}


void WriteConnections (tree *node, FILE *out)
{
	if (node->left == NULL && node->right == NULL) return;

	if (node->left != NULL)
	{
		fprintf (out, "\"%p\"->\"%p\" [style=\"bold\", color = \"royalblue\"];\n", node, node->left);
		WriteConnections (node->left, out);
	}

	if (node->right != NULL)
	{
		fprintf (out, "\"%p\"->\"%p\" [style=\"bold\", color = \"lawngreen\"];\n", node, node->right);
		WriteConnections (node->right, out);
	}
	return;
}


void Dump (tree *root, const char svg_file[MAX_FILE_NAME], const char mode)
{
	tree *node = root;
	FILE *out = fopen ("tree.dot", "w");

	if (mode == 'l')
	{
		fprintf (out, "digraph {\nnode[shape = record];\n");
		WriteLabels (root, out);
		if (node->left != NULL || node->right != NULL) WriteConnections (root, out);
		else fprintf (out, "%g;\n", node->data);
		fprintf(out, "}");
	}

	fclose (out);
	char command[53] = "dot tree.dot -T svg -o ";
	strcpy (&command[23], svg_file);
	system (command);
	return;
}


void PrintNode (tree *node, FILE *output)
{
	fprintf(output, "%c:", node->type);
	switch (node->type)
	{
		case CONSTANT:
			fprintf (output, "%g", node->data);
			break;

		case VARIABLE:
		{
			fprintf (output, "%s", variables[FindVar (node->data)].name);
			break;
		}

		default:
			switch ((int) node->data)
			{
				#define FUNCTION(name, diff_description, print_description) case name##NUM: fprintf(output, "%s", name); break;
				#define OPERATOR(name, diff_description, simp_description, print_description) case name##NUM: fprintf(output, "%s", name); break;
				#include "DSL/DSL_function_descriptions.h"
				#include "DSL/DSL_operator_descriptions.h"
				#undef FUNCTION
				#undef OPERATOR
				default:
					fprintf (output, "%s", functions[FindFunc (node->data)].name);
					break;
			}
			break;
	}

	if (node->left == NULL && node->right == NULL)
	{
		fprintf(output, "}");
		return;
	}

	if (node->left != NULL)
	{
		fprintf(output, "{");
		PrintNode (node->left, output);
	}
	else fprintf(output, "#");

	if (node->right != NULL)
	{
		fprintf(output, "{");
		PrintNode (node->right, output);
	}
	else fprintf(output, "#");

	fprintf (output, "}");
	return;
}


void PrintTree (tree *root, const char file_name[MAX_FILE_NAME])
{
	FILE *out = fopen (file_name, "w");
	fprintf(out, "{");
	PrintNode (root, out);
	fclose (out);
	return;
}


double ReadId (const char mode)
{
	double val = 0;
	double multiplier = 1;
	int i = 0;
	while (str[i] != '{' && str[i] != '}' && str[i] != '#')
	{
		if (mode == 'V') variables[free_var].name[i] = str[i];
		else if (mode == 'F') functions[free_func].name[i] = str[i];
		val += str[i] * multiplier;
		i++;
		multiplier *= 2;
		if (multiplier > MAX_FUNC_HASH_MULTIPLIER)
		{
			printf("Syntax error: unknown function %g\n", val);
			return 0;
		}
	}

	switch (mode)
	{
		case 'N':
			str += i;
			break;

		case 'V':
			str += i;
			variables[free_var].hash = (int) val;
			free_var++;
			break;

		case 'F':
			str += i;
			functions[free_func].hash = (int) val;
			free_func++;
			break;
	}
	return val;
}


tree *ReadNode ()
{
	char type = *str++;
	if (*str == ':') str++;
	double val = 0;
	switch (type)
	{
		case FUNCTION:
			if (FindFunc (ReadId ('R')) < 0) val = ReadId ('F');
			else val = ReadId ();
			break;

		case VARIABLE:
			if (FindVar (ReadId ('R')) < 0) val = ReadId ('V');
			else val = ReadId ();
			break;

		case CONSTANT:
			val = strtod (str, &str);
			break;

		case OPERATOR:
			val = ReadId ();
			break;
	}
	tree *node = CreateNode (type, val);
	switch (*str)
	{
		case '{':
			str++;
			node->left = ReadNode ();
			node->left->parent = node;
			if (*str == '#') str++;
			else if (*str == '{')
			{
				str++;
				node->right = ReadNode ();
				node->right->parent = node;
			}
			break;
		case '#':
			str += 2;
			node->right = ReadNode ();
			node->right->parent = node;
			break;
	}
	if (*str == '}') str++;
	return node;
}


tree *ReadTree (const char file_name[MAX_FILE_NAME])
{
	str = GetBuffer (file_name);
	if (str == NULL) return NULL;
	str++;
	tree *root = ReadNode ();
	return root;
}