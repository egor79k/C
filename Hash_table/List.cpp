#include <iostream>
#include <stdlib.h>
#include <string.h>

//#include "List.h"


#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif


class List
{
public:
	const int RESIZE_VALUE = 2;
	const int HYSTERESIS   = 2;
	const int START_LIST_SIZE = 2;

	char **data = {};
	int *next = {};
	int *prev = {};
	int head = 0;
	int tail = 0;
	int size = 0;
	int free_elem = 1;
	//bool sort = 1;

	List ();
	~List ();
	//bool ListConstruct (int length);
	void ListResize (int resize_val);
	//bool ListDestruct (List *lst);
	//bool ListFree (List *lst);

	int InsertAfter (int prev_num, char *value);
	//int InsertBefore (int next_num, int value);
	//bool Delete (int num);
	int FindByValue (char *value);
	bool CmpStr (char *str_1, char *str_2);
	//int FindByOrder (int order_num);
	//bool SortList (List *lst);

	//void Dump (const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
	//void GraphDump (char png_file[MAX_PNG_NAME], const char mode = 'l');
	int ListOk ();
	void CheckList ();
};



/**
*	List initialization
*
*	@param[in] lst List pointer
*	@param[in] name List name pointer
*	@param[in] length Size of new list
*
*	return true - Succesful initialisation false - Initialization error
*/
List::List ()
{
	size = START_LIST_SIZE + 1;
	data = (char **) calloc (size, sizeof (char *));
	next = (int *) calloc (size, sizeof (int));
	prev = (int *) calloc (size, sizeof (int));

	for (int i = 1; i < size - 1; i++)
	{
		next[i] = i + 1;
		prev[i] = -1;
	}
	prev[size - 1] = -1;

	ON_DEBUG(CheckList ();)
}

/**
*	Change list size
*
*	@param[in] lst List pointer
*	@param[in] resize_val Value of extension
*
*	return true - Succesful initialisation false - Initialization error
*/
void List::ListResize (int resize_val)
{
	size *= resize_val;
	data = (char **) realloc (data, size * sizeof (char *));
	next = (int *) realloc (next, size * sizeof (int));
	prev = (int *) realloc (prev, size * sizeof (int));

	if (resize_val > 0)
	{
		next[free_elem] = size / resize_val;

		for (int i = size / resize_val; i < size; i++)
		{
			data[i] = NULL;
			next[i] = i + 1;
			prev[i] = -1;
		}
		next[size - 1] = 0;
	}

	ON_DEBUG(CheckList ();)
	return;
}

/**
*	Destruct list
*
*	@param[in] lst List pointer
*
*	return true - succesful destruction false - destruction error
*/
List::~List ()
{
	free (data);
	free (next);
	free (prev);
	head = 0;
	tail = 0;
	free_elem = 0;
	//sort = false;
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
int List::InsertAfter (int prev_num, char *value)
{
	if (prev[prev_num] < 0) return 0;
	int new_num = free_elem;

	if (prev_num == 0 && head == 0) head = new_num;
	else if (prev_num == 0 && head != 0) return 0;
	else if (prev_num != 0 && head == 0) return 0;

	if (next[new_num] == 0) ListResize (RESIZE_VALUE);

	free_elem = next[free_elem];
	data[new_num] = value;
	next[new_num] = next[prev_num];
	prev[new_num] = prev_num;
	if (next[new_num] != 0) prev[next[new_num]] = new_num;
	else tail = new_num;
	if (prev_num != 0) next[prev_num] = new_num;

	//sort = false;

	ON_DEBUG(CheckList ();)
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
*//*
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
*/
/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return i - Number of finded value 0 - No such element in list
*/
int List::FindByValue (char *value)
{
	for (int i = head; i != 0; i = next[i])
	{
		if (CmpStr (data[i], value)) return i;
	}
	return 0;
}


bool List::CmpStr (char *str_1, char *str_2)
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
*//*
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
*/
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
int List::ListOk ()
{
	int i = head;
	while (next[i] != 0)
	{
		if (prev[i] < 0) return 1;
		i = next[i];
	}
	if (prev[i] < 0) return 1;
	if (i != tail) return 3;
	while (prev[i] > 0) i = prev[i];
	if (i != head) return 2;

	i = free_elem;
	while (i != 0)
	{
		if (prev[i] >= 0) return 4;
		i = next[i];
	}
	return 0;
}

/**
*	Printing list errors
*
*	@param[in] lst List pointer
*/
void List::CheckList ()
{
	int err = ListOk ();
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