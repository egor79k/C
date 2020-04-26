#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEBUG 0

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif

const int RESIZE_VALUE = 2;
const int HYSTERESIS   = 2;
const int START_LIST_SIZE = 2;


struct List
{
	char **data = {};
	int *next = {};
	int *prev = {};
	int head = 0;
	int tail = 0;
	int size = 0;
	int free_elem = 1;
};


bool ListConstruct (List *lst);
bool ListResize (List *lst, int resize_val);
void ListDestruct (List *lst);

int InsertAfter (List *lst, int prev_num, char *value);
bool Delete (List *lst, char *value);
int FindByValue (List *lst, char *value);
char *Find (List *lst, char *value);
int End (List *lst);

int ListOk (List *lst);
void CheckList (List *lst);


bool ListConstruct (List *lst)
{
	lst->size = START_LIST_SIZE + 1;
	lst->data = (char **) calloc (lst->size, sizeof (char *));
	lst->next = (int *) calloc (lst->size, sizeof (int));
	lst->prev = (int *) calloc (lst->size, sizeof (int));
	for (int i = 1; i < lst->size - 1; i++)
	{
		lst->next[i] = i + 1;
		lst->prev[i] = -1;
	}
	lst->prev[lst->size - 1] = -1;
	lst->free_elem = 1;

	ON_DEBUG(CheckList (lst);)
	return true;
}


/**
*	Change List size
*
*	@param[in] lst List pointer
*	@param[in] resize_val Value of extension
*
*	return true - Succesful initialisation false - Initialization error
*/
bool ListResize (List *lst, int resize_val)
{
	lst->size *= resize_val;
	lst->data = (char **) realloc (lst->data, lst->size * sizeof (char *));
	lst->next = (int *) realloc (lst->next, lst->size * sizeof (int));
	lst->prev = (int *) realloc (lst->prev, lst->size * sizeof (int));

	if (resize_val > 0)
	{
		lst->next[lst->free_elem] = lst->size / resize_val;

		for (int i = lst->size / resize_val; i < lst->size; i++)
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
*	Destruct List
*
*	@param[in] lst List pointer
*
*	return true - succesful destruction false - destruction error
*/
void ListDestruct (List *lst)
{
	free (lst->data);
	free (lst->next);
	free (lst->prev);
	return;
}


/**
*	Put element in List after pointed
*
*	@param[in] lst List pointer
*	@param[in] prev_num Previous number
*	@param[in] value Element
*
*	return true - Succesful putting false - Putting error
*/
int InsertAfter (List *lst, int prev_num, char *value)
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
	
	ON_DEBUG(CheckList (lst);)
	return new_num;
}


/**
*	Delete pointed element from List
*
*	@param[in] lst List pointer
*	@param[in] num Number
*
*	return true - Succesful delete false - Delete error
*/
bool Delete	(List *lst, char *value)
{
	int num = FindByValue (lst, value);
	if (num < 0) return false;
	if (lst->prev[num] < 0) return false;
	if (num == lst->head) lst->head = lst->next[num];
	if (num == lst->tail) lst->tail = lst->prev[num];
	if (lst->prev[num] != 0) lst->next[lst->prev[num]] = lst->next[num];
	if (lst->prev[lst->next[num]] != 0) lst->prev[lst->next[num]] = lst->prev[num];
	lst->next[num] = lst->free_elem;
	lst->free_elem = num;
	lst->data[num] = NULL;
	lst->prev[num] = -1;

	ON_DEBUG(CheckList (lst);)
	return true;
}


/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return i - Index of finded element; -1 - No such element in List
*/
int FindByValue (List *lst, char *value)
{
	for (int i = lst->head; i != 0; i = lst->next[i])
	{
		if (!strcmp(lst->data[i], value)) return i;
	}
	return -1;
}


/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return data[i] - Pointer to finded value; NULL - No such element in List
*/
char *Find (List *lst, char *value)
{
	int i = FindByValue (lst, value);
	if (i >= 0) return lst->data[i];
	return NULL;
}


/**
*	Return List tail
*
*	return tail - List end
*/
int End (List *lst)
{
	return lst->tail;
}

//Errors: 0 - No errors, 1 - Empty value in prev in full element, 2 - List order is broken, 3 - Wrong pointed tail, 4 - Wrong defined free element

/**
*	Checking List for errors
*
*	@param[in] lst List pointer
*
*	return 0 - No errors, 1 - Empty value in prev in full element, 2 - List order is broken, 3 - Wrong pointed tail, 4 - Wrong defined free element
*/
int ListOk (List *lst)
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
*	Printing List errors
*
*	@param[in] lst List pointer
*/
void CheckList (List *lst)
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
	}
}