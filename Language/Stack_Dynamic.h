#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
	#define ASSERT_OK 														\
	if (StackOK (stk) == false)												\
	{																		\
		Dump (stk, "Assertion error", DEFAULT);								\
		stk->err = 0;														\
		stk->hash = CountHash (stk);										\
	}
#else
	#define ON_DEBUG(code) ;
	#define ASSERT_OK ;
#endif

typedef int Elem_t;

const int RESIZE_VALUE = 5;
const int HYSTERESIS = 2;
const int FREE_PLACE = -666;
const int MAX_NAME_LENGTH = 20;
ON_DEBUG(const int Canary1_value = 1234567890;)
ON_DEBUG(const int Canary2_value = 1876543290;)


struct stack_t
{
	ON_DEBUG(int Canary1 = Canary1_value;)
	Elem_t *data;							//Massive of elements
	int full_size;							//Size of stack
	int size;								//Current position in stack
	int err;								//Errors: 0 - No errors, 1 - Stack Owerflow, 2 - Stack Underflow, 3 - Stack Attacked
	const char *name;						//Name of stack
	ON_DEBUG(int hash;)
	ON_DEBUG(int Canary2 = Canary2_value;)
};

//Work with stack:
bool StackConstruct (stack_t *stk, const int start_size = 1, const char * stk_name = {"Unnamed stack"});
bool StackResize (stack_t *stk, int resize_val);
bool StackPush (stack_t *stk, Elem_t value);
Elem_t StackPop (stack_t *stk);
bool StackDestruct (stack_t *stk);
//Check stack:
void Dump (stack_t *stk, const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
bool StackOK (stack_t *stk);
int CountHash (stack_t *stk);


