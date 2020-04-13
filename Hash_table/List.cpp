#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

/**
*	List initialization
*
*	@param[in] lst List pointer
*	@param[in] name List name pointer
*	@param[in] length Size of new list
*
*	return true - Succesful initialisation false - Initialization error
*/
bool ListConstruct (list *lst, int length)
{
	lst->size = length + 1;
	lst->data = (char **) calloc (lst->size, sizeof (char *));
	lst->next = (int *) calloc (lst->size, sizeof (int));
	lst->prev = (int *) calloc (lst->size, sizeof (int));

	for (int i = 1; i < lst->size - 1; i++)
	{
		lst->next[i] = i + 1;
		lst->prev[i] = -1;
	}
	lst->prev[lst->size - 1] = -1;

	ON_DEBUG(CheckList (lst);)
	return true;
}

/**
*	Change list size
*
*	@param[in] lst List pointer
*	@param[in] resize_val Value of extension
*
*	return true - Succesful initialisation false - Initialization error
*/
bool ListResize (list *lst, int resize_val)
{
	lst->size += resize_val;
	lst->data = (char **) realloc (lst->data, lst->size * sizeof (char *));
	lst->next = (int *) realloc (lst->next, lst->size * sizeof (int));
	lst->prev = (int *) realloc (lst->prev, lst->size * sizeof (int));

	if (resize_val > 0)
	{
		lst->next[lst->free_elem] = lst->size - resize_val;

		for (int i = lst->size - resize_val; i < lst->size; i++)
		{
			lst->data[i] = NULL;
			lst->next[i] = i + 1;
			lst->prev[i] = -1;
		}
		lst->next[lst->size - 1] = 0;
	}

	ON_DEBUG(CheckList (lst);)
	return true;
}

/**
*	Destruct list
*
*	@param[in] lst List pointer
*
*	return true - succesful destruction false - destruction error
*/
bool ListDestruct (list *lst)
{
	free (lst->data);
	free (lst->next);
	free (lst->prev);
	lst->head = 0;
	lst->tail = 0;
	lst->free_elem = 0;
	lst->sort = false;
	return true;
}

/**
*	Free list
*
*	@param[in] lst List pointer
*
*	return true - succesful free false - free error
*//*
bool ListFree (list *lst)
{
	memset (lst->data, 0, lst->size * sizeof (int));

	for (int i = 1; i < lst->size; i++)
	{
		lst->next[i] = i + 1;
		lst->prev[i] = -1;
	}
	lst->next[lst->size - 1] = 0;

	lst->head = 0;
	lst->tail = 0;
	lst->free_elem = 1;
	lst->sort = true;

	ON_DEBUG(CheckList (lst);)
	return true;
}
*/
/**
*	Put element in list after pointed
*
*	@param[in] lst List pointer
*	@param[in] prev_num Previous number
*	@param[in] value Element
*
*	return true - Succesful putting false - Putting error
*/
int InsertAfter (list *lst, int prev_num, char *value)
{
	if (lst->prev[prev_num] < 0) return 0;
	int new_num = lst->free_elem;

	if (prev_num == 0 && lst->head == 0) lst->head = new_num;
	else if (prev_num == 0 && lst->head != 0) return 0;
	else if (prev_num != 0 && lst->head == 0) return 0;

	if (lst->next[new_num] == 0) ListResize (lst, RESIZE_VALUE);

	lst->free_elem = lst->next[lst->free_elem];
	lst->data[new_num] = value;
	lst->next[new_num] = lst->next[prev_num];
	lst->prev[new_num] = prev_num;
	if (lst->next[new_num] != 0) lst->prev[lst->next[new_num]] = new_num;
	else lst->tail = new_num;
	if (prev_num != 0) lst->next[prev_num] = new_num;

	lst->sort = false;

	ON_DEBUG(CheckList (lst);)
	return new_num;
}

/**
*	Put element in list before pointed
*
*	@param[in] lst List pointer
*	@param[in] next_num Next number
*	@param[in] value Element
*
*	return true - Succesful putting false - Putting error
*//*
int InsertBefore (list *lst, int next_num, int value)
{
	if (next_num == 0) return 0;
	if (lst->prev[next_num] < 0) return 0;
	int new_num = lst->free_elem;

	if (next_num == lst->head)
	{
		if (lst->next[new_num] == 0) ListResize (lst, RESIZE_VALUE);
		lst->free_elem = lst->next[lst->free_elem];
		lst->data[new_num] = value;
		lst->next[new_num] = next_num;
		lst->prev[new_num] = lst->prev[next_num];
		lst->head = new_num;
		lst->prev[next_num] = new_num;
		lst->sort = 0;
	}
	else
	{
		new_num = InsertAfter (lst, lst->prev[next_num], value);
	}
	
	ON_DEBUG(CheckList (lst);)
	return new_num;
}*/

/**
*	Delete pointed element from list
*
*	@param[in] lst List pointer
*	@param[in] num Number
*
*	return true - Succesful delete false - Delete error
*/
bool Delete	(list *lst, int num)
{
	if (lst->prev[num] < 0) return false;
	if (num == lst->head) lst->head = lst->next[num];
	if (num == lst->tail) lst->tail = lst->prev[num];
	if (lst->prev[num] != 0) lst->next[lst->prev[num]] = lst->next[num];
	if (lst->prev[lst->next[num]] != 0) lst->prev[lst->next[num]] = lst->prev[num];
	lst->next[num] = lst->free_elem;
	lst->free_elem = num;
	lst->data[num] = NULL;
	lst->prev[num] = -1;

	if (num != lst->tail) lst->sort = false;

	ON_DEBUG(CheckList (lst);)
	return true;
}

/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return i - Number of finded value 0 - No such element in list
*/
int FindByValue (list *lst, char *value)
{
	for (int i = lst->head; i != 0; i = lst->next[i])
	{
		if (CmpStr (lst->data[i], value)) return i;
	}
	return 0;
}


bool CmpStr (char *str_1, char *str_2)
{
	while (*str_1 != '\0' && *str_2 != '\0')
	{
		if (*str_1 == *str_2)
		{
			str_1++;
			str_2++;
		}
		else return false;
	}
	return true;
}

/**
*	Finding number of pointed element by it's logic num
*
*	@param[in] lst List pointer
*	@param[in] order_num Order number of element
*
*	return i - Number of finded value 0 - No such element in list
*//*
int FindByOrder (list *lst, int order_num)
{
	if (order_num <= 0) return 0;
	if (lst->sort == true) return order_num;
	else
	{
		int k = lst->head;
		for (int i = 1; i < order_num; i++)
		{
			k = lst->next[k];
			if (k == 0) return 0;
		}
		return k;
	}
}*/

/**
*	Sorting list by logic numbers
*
*	@param[in] lst List pointer
*
*	return true - Succesful sort false - Sort error
*//*
bool SortList (list *lst)
{
	int i = lst->head;
	int k = 0;

	while (i != 0)
	{
		lst->prev[i] = k;
		i = lst->next[i];
		k++;
	}

	for (i = 1; i < lst->size; i++)
	{
		if (lst->prev[i] < 0) lst->prev[i] = 2147483647;
	}

	for (i = 1; i < lst->size; i++)
	{
		for (int j = 1; j < lst->size - 1; j++)
		{
			if (lst->prev[j] > lst->prev[j + 1])
			{
				int tmp = lst->prev[j + 1];
				lst->prev[j + 1] = lst->prev[j];
				lst->prev[j] = tmp;

				char **tmp_data = lst->data[j + 1];
				lst->data[j + 1] = lst->data[j];
				lst->data[j] = tmp_data;
			}
		}
	}

	for (i = 1; i < k; i++)
	{
		printf("%d\n", lst->prev[i]);
		lst->next[i] = i + 1;
	}
	lst->next[k] = 0;

	lst->head = 1;
	lst->tail = k;

	lst->free_elem = k + 1;
	for (i = k + 1; i < lst->size; i++)
	{
		lst->next[i] = i + 1;
		lst->prev[i] = -1;
	}
	lst->next[lst->size - 1] = 0;

	lst->sort = true;
	return true;

	ON_DEBUG(CheckList (lst);)
}*/

/**
*	Dumping list
*
*	@param[in] lst List pointer
*	@param[in] reason Dump reason
*	@param[in] file File name where dump from
*	@param[in] line Line name where dump from
*	@param[in] function Function name where dump from
*/
void Dump (list *lst, const char *reason, const char *file, const int line, const char *function)
{
	int i = 0;
	printf("\n============================================================\n");
	printf("%s from %s (%d) %s()\n\n|List: [%p] ", reason, file, line, function, lst);
	if (ListOk (lst) == 0) printf("(Ok)\n");
	else printf("(Error)\n");

	for (i = lst->head; i != 0; i = lst->next[i])
	{
		printf("|[%d] = %s\n", i, lst->data[i]);
	}

	printf ("\n%c", 218);
	for (i = 0; i < lst->size * 5 + 6; i++)
	{
		putchar (196);
	}

	printf ("%c\n%cnum:  ", 191, 179);
	for (i = 0; i < lst->size; i++)
	{
		printf ("%5d", i);
	}

	printf ("%c\n%c", 179, 195);
	for (i = 0; i < lst->size * 5 + 6; i++)
	{
		putchar (196);
	}

	printf ("%c\n%cdata: ", 180, 179);
	for (i = 0; i < lst->size; i++)
	{
		printf ("%5s", lst->data[i]);
	}

	printf ("%c\n%cnext: ", 179, 179);
	for (i = 0; i < lst->size; i++)
	{
		printf ("%5d", lst->next[i]);
	}

	printf ("%c\n%cprev: ", 179, 179);
	for (i = 0; i < lst->size; i++)
	{
		printf ("%5d", lst->prev[i]);
	}

	printf("%c\n%c", 179, 192);
	for (i = 0; i < lst->size * 5 + 6; i++)
	{
		putchar (196);
	}
	printf("%c\n|head: %d", 217, lst->head);
	printf("\n|tail: %d", lst->tail);
	printf("\n|size: %d", lst->size);
	printf("\n|free: %d", lst->free_elem);
	printf("\n|sort: %d\n", lst->sort);
	printf("============================================================\n");
	return;
}

/**
*	Dumping list content in image
*
*	@param[in] lst List pointer
*	@param[in] png_file Dumping file name
*//*
void GraphDump (list *lst, char png_file[MAX_PNG_NAME], const char mode)
{
	int i = lst->head;
	int elem = 0;
	FILE *out = fopen ("list.dot", "w");


	if (mode == 's')
	{
		fprintf (out, "digraph {\nedge[color = \"blue\"];\nnode[color = \"green4\", fontsize = 20];\nrankdir=LR;\n");
		fprintf (out, "name[shape = doubleoctagon, label = , color = \"goldenrod\", fontsize = 20];\n");
		fprintf (out, "name->%d[color = \"white\", arrowhead = \"none\"];\n", lst->data[i]);
		
	while (lst->next[i] != 0)
	{
		fprintf (out, "%d->", lst->data[i]);
		i = lst->next[i];
	}
	fprintf (out, "%d;\n", lst->data[i]);

	fprintf (out, "}");
	}


	if (mode == 'l')
	{
		char *occupied = "olivedrab1";
		char *free = "lightgrey";
		char *zero = "mintcream";
		char *head = "paleturquoise";
		char *tail = "moccasin";
		char *color = "";

		fprintf (out, "digraph {\nnode[shape = record];\nrankdir=LR;\n");

		for (i = 0; i < lst->size; i++)
		{
			if (lst->prev[i] == -1) color = free;
			else color = occupied;
			if (i == 0) color = zero;
			else if (i == lst->head) color = head;
			else if (i == lst->tail) color = tail;

			fprintf (out, "%d[label = \"<n> next\\n%d | %d | num\\n%d | adress\\n%p | <p> prev\\n%d\", style=\"filled\", fillcolor = \"%s\"];\n", i, lst->next[i], lst->data[i], i, &lst->data[i], lst->prev[i], color);
			elem++;
		}
		fprintf(out, "name[shape = doubleoctagon, label = , color = \"goldenrod\", fontsize = 20];\n");
		fprintf(out, "params[shape = record, label = \" head\\n%d | tail\\n%d | free_elem\\n%d | size\\n%d | sort\\n%d | adress\\n%p\", color = \"goldenrod\"];\n", lst->head, lst->tail, lst->free_elem, lst->size, lst->sort, lst);
		fprintf (out, "name->params->0[arrowhead = \"none\", color = \"white\"];\n");

		for (i = 0; i < elem - 1; i++)
		{
			fprintf(out, "%d->", i);
		}
		fprintf (out, "%d[arrowhead = \"none\", color = \"white\"];\n", i);

		i = lst->head;
		while (lst->next[i] != 0)
		{
			fprintf (out, "%d:<n>->", i);
			i = lst->next[i];
		}
		fprintf (out, "%d:<n>[color = \"red\"];\n", i);

		while (lst->prev[i] != 0)
		{
			fprintf (out, "%d:<p>->", i);
			i = lst->prev[i];
		}
		fprintf (out, "%d:<p>[color = \"blue\"];\n", i);

		fprintf (out, "}");
	}

	fclose (out);

	char command[53] = "dot list.dot -T png -o ";
	strcpy (&command[23], png_file);
	system (command);
	return;
}
*/
//Errors: 0 - No errors, 1 - Empty value in prev in full element, 2 - List order is broken, 3 - Wrong pointed tail, 4 - Wrong defined free element

/**
*	Checking list for errors
*
*	@param[in] lst List pointer
*
*	return 0 - No errors, 1 - Empty value in prev in full element, 2 - List order is broken, 3 - Wrong pointed tail, 4 - Wrong defined free element
*/
int ListOk (list *lst)
{
	int i = lst->head;
	while (lst->next[i] != 0)
	{
		if (lst->prev[i] < 0) return 1;
		i = lst->next[i];
	}
	if (lst->prev[i] < 0) return 1;
	if (i != lst->tail) return 3;
	while (lst->prev[i] > 0) i = lst->prev[i];
	if (i != lst->head) return 2;

	i = lst->free_elem;
	while (i != 0)
	{
		if (lst->prev[i] >= 0) return 4;
		i = lst->next[i];
	}
	return 0;
}

/**
*	Printing list errors
*
*	@param[in] lst List pointer
*/
void CheckList (list *lst)
{
	int err = ListOk (lst);
	if (err != 0)
	{
		switch (err)
		{
			case 1:
				printf("Error: Empty value of previous of used element\n");
				break;
			case 2:
				printf("Error: List order is broken");
				break;
			case 3:
				printf("Error: Wrong pointed tail\n");
				break;
			case 4:
				printf("Error: Wrong defined free element\n");
				break;
		}
		//Dump (lst, "Error", DEFAULT);
	}
}