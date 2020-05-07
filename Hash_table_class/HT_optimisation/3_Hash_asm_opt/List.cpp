#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#define DEBUG 0

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif


class List
{
private:
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

	int FindByValue (char *value);
	void ListResize (int resize_val);
	int ListOk ();
	void CheckList ();

public:
	List ();
	~List ();

	int InsertAfter (int prev_num, char *value);
	bool Delete (char *value);
	char *Find (char *value);
	int End ();
};



/**
*	List constructor
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
*	List destructor
*/
List::~List ()
{
	free (data);
	free (next);
	free (prev);
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

	ON_DEBUG(CheckList ();)
	return new_num;
}


/**
*	Delete pointed element from list
*
*	@param[in] lst List pointer
*	@param[in] num Number
*
*	return true - Succesful delete false - Delete error
*/
bool List::Delete (char *value)
{
	int num = FindByValue (value);
	if (num < 0) return false;
	if (prev[num] < 0) return false;
	if (num == head) head = next[num];
	if (num == tail) tail = prev[num];
	if (prev[num] != 0) next[prev[num]] = next[num];
	if (prev[next[num]] != 0) prev[next[num]] = prev[num];
	next[num] = free_elem;
	free_elem = num;
	data[num] = NULL;
	prev[num] = -1;

	ON_DEBUG(CheckList ();)
	return true;
}


/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return i - Index of finded element; -1 - No such element in list
*/
int List::FindByValue (char *value)
{
	for (int i = head; i != 0; i = next[i])
	{
		if (!strcmp(data[i], value)) return i;
	}
	return -1;
}


/**
*	Finding number of pointed element
*
*	@param[in] lst List pointer
*	@param[in] value Value which finding
*
*	return data[i] - Pointer to finded value; NULL - No such element in list
*/
char *List::Find (char *value)
{
	int i = head;

	while (i != 0)
	{
		int not_equal = 0;

		asm (".intel_syntax noprefix\n"
			"	xor rax, rax\n"
			"	mov bl, 0\n"
			"	cld\n"

			".compare:\n"
			"	cmpsb\n"
			"	jne .endcmp\n"
			"	dec rsi\n"
			"	lodsb\n"
			"	cmp bl, al\n"
			"	jne .compare\n"

			".endcmp:\n"
			"	mov al, [rdi - 1]\n"
			"	sub al, [rsi - 1]\n"
			".att_syntax prefix\n"
			
			: "=a"(not_equal)
			: "D"(value), "S"(data[i])
			: "rbx"
			);

		if (!not_equal)
		{
			return data[i];
		}

		i = next[i];
	}
/*
	int i = 0;
	int j = 0;

	for (i = head; i != 0; i = next[i])
	{
		j = 0;
		while (data[i][j] != '\0' && value[j] != '\0' && data[i][j] == value[j]) j++;
		if (data[i][j] == '\0' && value[j] == '\0') return data[i];
		//if (!strcmp(data[i], value)) break;
	}

	//if (i >= 0) return data[i];
*/
	return NULL;
}


/**
*	Return list tail
*
*	return tail - list end
*/
int List::End ()
{
	return tail;
}

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
	}
}