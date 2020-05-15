#include "Stack_Dynamic.h"

/**
*	Stack initialization
*
*	@param[in] stk Stack pointer
*	@param[in] start_size Size of new stack
*	@param[in] stk_name Stack name pointer
*
*	return true - Succesful initialisation false - Initialization error
*/
bool StackConstruct (stack_t *stk, const int start_size, const char * stk_name)
{
	stk->data = (Elem_t *) calloc (start_size, sizeof (Elem_t));
	for (int i = 0; i < start_size; i++) stk->data[i] = FREE_PLACE;
	stk->full_size = start_size;
	stk->size = 0;
	stk->err = 0;
	stk->name = stk_name;
	ON_DEBUG(stk->hash = CountHash (stk);)
	ON_DEBUG(stk->hash = CountHash (stk);)
	ASSERT_OK
	return true;
}

/**
*	Change stack size
*
*	@param[in] stk Stack pointer
*	@param[in] resize_val Value of extension
*
*	return true - Succesful initialisation false - Initialization error
*/
bool StackResize (stack_t *stk, int resize_val)
{
	stk->full_size = stk->full_size + resize_val;
	stk->data = (Elem_t *) realloc (stk->data, stk->full_size * sizeof (Elem_t));
	if (resize_val > 0)
	{
		for (int i = stk->full_size - resize_val; i < stk->full_size; i++)
		{
			stk->data[i] = FREE_PLACE;
		}
	}
	ON_DEBUG(stk->hash = CountHash (stk);)
	ASSERT_OK
	return true;
}

/**
*	Put element in stack
*
*	@param[in] stk Stack pointer
*	@param[in] value Element
*
*	return true - Succesful initialisation false - Initialization error
*/
bool StackPush (stack_t *stk, Elem_t value)
{
	ASSERT_OK
	if (stk->size < stk->full_size)
	{
		stk->data[stk->size++] = value;
	}
	else
	{
		StackResize (stk, RESIZE_VALUE);
		stk->data[stk->size++] = value;
	}
	ON_DEBUG(stk->hash = CountHash (stk);)
	ASSERT_OK
	return true;
}

/**
*	Takes element from stack
*
*	@param[in] stk Stack pointer
*
*	return Element
*/
Elem_t StackPop (stack_t *stk)
{
	ASSERT_OK
	if (stk->size == 0)
	{
		stk->err = 2;
		ASSERT_OK
		return stk->data[stk->size];
	}

	Elem_t value = stk->data[--stk->size];
	stk->data[stk->size] = FREE_PLACE;

	if (stk->size < stk->full_size - RESIZE_VALUE - HYSTERESIS && stk->full_size > RESIZE_VALUE)
	{
		StackResize (stk, -RESIZE_VALUE);
	}
	ON_DEBUG(stk->hash = CountHash (stk);)
	ASSERT_OK
	return value;
}

/**
*	Destruct stack
*
*	@param[in] stk Stack pointer
*
*	return true - succesful destruction false - destruction error
*/
bool StackDestruct (stack_t *stk)
{
	ASSERT_OK
	ON_DEBUG(stk->Canary2 = 0;)
	free (stk->data);
	stk->full_size = 0;
	stk->size = 0;
  	stk->err = 0;
	stk->name = nullptr;
	ON_DEBUG(stk->hash = 0;)
	ON_DEBUG(stk->Canary2 = 0;)

  return true;
}

/*
*	Print currrent information about stack
*
*	@param[in] stk Stack pointer
*	@param[in] reason String pointer with reason of dump
*	@param[in] file File name string pointer where dump from
*	@param[in] line Line number where dump from
*	@param[in] function Function name string pointer where dump from
*/
void Dump (stack_t *stk, const char *reason, const char *file, const int line, const char *function)
{
	printf("\n==============================\n");

	printf("%s from %s (%d) %s()\n\n|Stack_t \"%s\" [%p] ", reason, file, line, function, stk->name, stk);
	if (StackOK (stk) == true) printf("(Ok)\n");
	else printf("(Error)\n");
	printf("||size = %d\n||data[%d] = [%p]\n\n", stk->size, stk->full_size, &stk->data[stk->full_size]);

	for (int i = 0; i < stk->size; i++)
	{
		printf ("*[%d] = %d", i, stk->data[i]);
		if (stk->data[i] == FREE_PLACE) printf(" [Poison]\n");
		else printf("\n");
	}

	for (int i = stk->size; i < stk->full_size; i++)
	{
		printf (" [%d] = %d", i, stk->data[i]);
		if (stk->data[i] == FREE_PLACE) printf(" [Poison]\n");
		else printf("\n");
	}

	if (stk->err == 0) printf ("\nerror = %d %s\n", stk->err, "(Ok)");
	else if (stk->err == 1) printf("\nerror = %d %s\n", stk->err, "(Stack Owerflow)");
	else if (stk->err == 2) printf("\nerror = %d %s\n", stk->err, "(Stack Underflow)");
	else printf("\nerror = %d %s\n", stk->err, "(Stack Attacked)");

	ON_DEBUG(printf("Hash = %d\n", stk->hash);)

	printf("==============================\n");
	return;
}

/**
*	Check stack
*
*	@param[in] stk Stack pointer
*
*	return true - stack is ok false - stack has an error
*/
bool StackOK (stack_t *stk)
{
	if (stk == nullptr) return false;
	if (stk->size > stk->full_size) stk->err = 1; 
	if (stk->size < 0) stk->err = 2;
	if (stk->data == nullptr) return false;
	if (stk->err != 0) return false;
	for (int i = stk->size; i < stk->full_size; i++)
	{
		if (stk->data[i] != FREE_PLACE) return false;
	}

	#ifdef DEBUG
	if (stk->Canary1 != Canary1_value)
	{
		stk->err = 3;
		return false;
	}

	if (stk->Canary2 != Canary2_value)
	{
		stk->err = 3;
		return false;
	}
	if (stk->hash != CountHash (stk))
	{
		stk->err = 3;
		return false;
	}
#endif
	return true;
}

/**
*	Count hash of stack
*
*	@param[in] stk Stack pointer
*
*	return hash Hash of stack
*/
int CountHash (stack_t *stk)
{
	int hash = 0;
	for (int i = 0; i < stk->size; i++)
	{
		for (int k = 0; k < sizeof (Elem_t); k++) hash = hash * 2 + (char) *(&stk->data[i] + k);
	}
	for (int k = 0; k < sizeof (int); k++) hash = hash * 2 + (char) *(&stk->full_size + k);
	for (int k = 0; k < sizeof (int); k++) hash = hash * 2 + (char) *(&stk->size + k);
	for (int k = 0; k < sizeof (int); k++) hash = hash * 2 + (char) *(&stk->err + k);
	for (int i = 0; i < MAX_NAME_LENGTH; i++)
	{
		hash = hash * 2 + stk->name[i];
	}
	return hash;
}