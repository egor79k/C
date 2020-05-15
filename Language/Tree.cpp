	///////////////////////////////////
   /// Tree with infix operations  ///
  /// (for Differentiator)        ///
 ///////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tree.h"
#include "DSL/DSL_definitions.h"


tree *TreeConstruct (char type, elem_t value)
{
	CHECK_TYPE
	tree *root = (tree *) calloc (1, sizeof (tree));
	root->data = value;
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL;
	root->level = 1;
	root->type = type;
	ON_DEBUG(CheckTree (root);)
	return root;
}


tree *CreateNode (char type, elem_t value, tree *left, tree *right)
{;
	tree *node = (tree *) calloc (1, sizeof (tree));
	node->data = value;
	
	if (left != NULL)
	{
		node->left = left;
		node->left->parent = node;
	}
	else node->left = NULL;

	if (right != NULL)
	{
		node->right = right;
		node->right->parent = node;
	}
	else node->right = NULL;

	node->type = type;
	return node;
}


tree *InsertLeft (tree *parent, char type, elem_t value)
{
	CHECK_TYPE
	if (parent == NULL) return NULL;

	if (parent->left == NULL)
	{
		parent->left = CreateNode (type, value);
		parent->left->parent = parent;
		ON_DEBUG(CheckTree (parent);)
		return parent->left;
	}
	else return NULL;
}


tree *InsertRight (tree *parent, char type, elem_t value)
{
	CHECK_TYPE
	if (parent == NULL) return NULL;
	if (parent->right == NULL)
	{
		parent->right = CreateNode (type, value);
		parent->right->parent = parent;
		ON_DEBUG(CheckTree (parent);)
		return parent->right;
	}
	else return NULL;
}


void DeleteNode (tree *node)
{
	if (node != NULL)
	{
		if (node->left != NULL) DeleteNode (node->left);
		if (node->right != NULL) DeleteNode (node->right);
		free (node);
	}
	return;
}


void Delete (tree *node)
{
	if (node->parent != NULL)
	{
		if (node->parent->left == node) node->parent->left = NULL;
		else node->parent->right = NULL;
	}
	DeleteNode (node);
	return;
}


void CopyNode (tree *node, tree *node_new)
{
	if (node->left != NULL)
	{
		InsertLeft (node_new, node->left->type, node->left->data);
		CopyNode (node->left, node_new->left);
	}

	if (node->right != NULL)
	{
		InsertRight (node_new, node->right->type, node->right->data);
		CopyNode (node->right, node_new->right);
	}

	return;
}


tree *Copy (tree *root)
{
	//Warning! New copied branch will be returned with NULL parent pointer.
	if (root == NULL) return NULL;
	tree *root_new = TreeConstruct (root->type, root->data);
	root_new->level = root->level;
	CopyNode (root, root_new);
	return root_new;
}


tree *FindNode (tree *node, elem_t value)
{
	if (node == NULL) return NULL;

	if (node->data == value) return node;

	if (node->left != NULL) node = FindNode (node->left, value);

	if (node->data != value && node->right != NULL) node = FindNode (node->right, value);

	if (node->data == value) return node;

	return node->parent;
}


/*void PrintNode (tree *node, FILE *output)
{
	if (node->left != NULL)
	{
		fprintf(output, "(");
		PrintNode (node->left, output);
	}

	switch (node->type)
	{
		case OPERATOR:
			fprintf(output, " %c ", (char) node->data);
			break;

		case FUNCTION:
		{
			switch ((int) node->data)
			{
				#define FUNCTION(name, diff_description, print_description) case name##NUM: fprintf(output, "%s ", name); break;
				#include "DSL\DSL_function_descriptions.h"
				#undef FUNCTION
			}
			break;
		}

		case VARIABLE:
			fprintf(output, "%c", (char) node->data);
			break;

		case CONSTANT:
			fprintf(output, "%g", node->data);
			break;
	}

	if (node->left == NULL && node->right == NULL)
	{
		fprintf(output, ")");
		return;
	}

	if (node->right != NULL)
	{
		fprintf(output, "(");
		PrintNode (node->right, output);
	}

	fprintf (output, ")");
	return;
}


void PrintTree (tree *root, char file_name[MAX_FILE_NAME])
{
	FILE *out = fopen (file_name, "w");
	fprintf(out, "(");
	PrintNode (root, out);
	fclose (out);
	return;
}*/

/*
char *Get_Buff_From_File (char file_name[MAX_FILE_NAME])
{
	FILE *in = fopen (file_name, "r");
	fseek (in, 0, SEEK_END);
	int NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	return text;
}
*/

/*void WriteLabels (tree *node, FILE *out)
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
			fprintf (out, "%s", variables[FindName (node->data)].name);
			break;
		}

		default:
			switch ((int) node->data)
			{
				#define FUNCTION(name, diff_description, print_description) case name##NUM: fprintf(out, "%s", name); break;
				#define OPERATOR(name, diff_description, simp_description, print_description) case name##NUM: fprintf(out, "%s", name); break;
				#include "DSL\DSL_function_descriptions.h"
				#include "DSL\DSL_operator_descriptions.h"
				#undef FUNCTION
				#undef OPERATOR
			}
			//fprintf (out, "%c", (char) node->data);
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


void Dump (tree *root, char png_file[MAX_FILE_NAME], const char mode)
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
	char command[53] = "dot tree.dot -T png -o ";
	strcpy (&command[23], png_file);
	system (command);
	return;
}*/


int TreeOk (tree *node)
{
	if (node->parent != NULL && node->level != node->parent->level + 1) return 3;
	if (node->left != NULL)
	{
		if (node->left->parent != node) return 1;
		return TreeOk (node->left);
	}
	if (node->right != NULL)
	{
		if (node->right->parent != node) return 2;
		return TreeOk (node->right);
	}
	return 0;
}


void CheckTree (tree *root)
{
	switch (TreeOk (root))
	{
		case 0:
			break;

		case 1:
			printf ("Error: Parent of one of the left nodes wrong defined.");
			break;

		case 2:
			printf ("Error: Parent of one of the right nodes wrong defined.");
			break;

		case 3:
			printf ("Error: Level of one of the nodes wrong defined.");
			break;
	}
	return;
}