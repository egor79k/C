#include <stdio.h>
#include "Commands.h"
#include "Common.h"

#define IS_IDENTIFICATOR ('a' <= *buffer && *buffer <= 'z') || ('A' <= *buffer && *buffer <= 'Z') || ('0' <= *buffer && *buffer <= '9') || *buffer == '_'
#define TRNS_ERROR "\e[1;35mTranslator: \e[31merror:\e[0m"

char *buffer = 0;
const int xor_hash_val = 1111111111;


int Identificator ()
{
	int hash = 0;

	while (IS_IDENTIFICATOR)
	{
		hash = hash xor xor_hash_val;
		hash += *buffer;
		buffer++;
	}

	return hash & 0xFFF;
}

FILE *output = 0;

int FindVar (int hash);
int FindFunc (int hash);

double GetId (const char mode = 'N');
//tree *GetProg ();
void GetD ();
tree *GetDef ();
tree * GetVar ();
tree *GetVarList (char mode = 'N');
tree *GetB ();
tree *GetOp ();
tree *GetIf ();
tree *GetC ();
tree *GetWhile ();
tree *GetAssn ();
tree *GetCall ();
tree *GetE ();	// Get == != > >= < <=
tree *GetS ();	// Get + -
tree *GetT ();	// Get * /
tree *GetPow ();// Get ^
tree *GetP ();	// Get ( )
tree *GetF ();	// Get function
tree *GetN ();	// Get num
tree *GetV ();	// Get variable


int main ()
{
	if (argc < 3)
	{
		printf("%s no input or/and output file\n", TRNS_ERROR);
		return 1;
	}
	printf("Translator called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);

	buffer = GetBuffer (argv[1]);
	output = fopen (argv[2], "w");
	printf("|Operating...\n");
	GetD ();
	return 0;
}


void GetD ()
{
	while (*buffer != '\0')
	{
		switch (GetId())
		{
			case L_DEF_NUM:
				GetDef ();
				break;

			case L_VAR_NUM:
				GetVar ();
				break;

			default:
				printf ("\nUndefined declaration \"%g\"\n", val);
				exit (1);
				break;
		}
	}
	return;
}


void GetDef ()
{
	double name = GetId ('F');
	SkipEmpty ();
	tree *args = GetVarList ('D');
	SkipEmpty ();
	return OPER(DEF_NUM, args, FUNC(name, GetB()));
}


void SkipWord ()
{
	while (IS_IDENTIFICATOR)
		buffer++;
	return;
}


hash GetId (const char mode)
{
	int hash = 0;
	int i = 0;
	while (IS_IDENTIFICATOR)
	{
		if (mode == 'V') variables[free_var].name[i] = buffer[i];
		else if (mode == 'F') functions[free_func].name[i] = buffer[i];
		val += buffer[i] * multiplier;
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
		case 'S':
			buffer += i;
			break;

		case 'V':
			buffer += i;
			variables[free_var].hash = (int) val;
			free_var++;
			break;

		case 'F':
			buffer += i;
			functions[free_func].hash = (int) val;
			free_func++;
			break;
	}
	return val;
}


tree * GetVar ()
{
	SkipWord ();
	SkipEmpty ();
	double name = GetId ('V');
	SkipEmpty ();
	if (L_ASSN_NUM == GetId ())
	{
		SkipWord ();
		SkipEmpty ();
		return OPER(VAR_NUM, GetE (), VAR(name));
	}
	else return OPER(VAR_NUM, NULL, VAR(name));
}


tree *GetVarList (char mode)
{
	if (L_ARGS_START_NUM != (int) GetId ('S')) printf("Error: expected \"%s\" after \"Def %s\" function declaration", L_ARGS_START_, functions[free_func - 1].name);
	SkipEmpty ();
	tree *root = NULL;
	if (*buffer != L_ARGS_END_SIGN)
	{
		if (mode == 'D') root = OPER(VARLIST_NUM, NULL, GetVar ());
		else root = OPER(VARLIST_NUM, NULL, GetE ());
		tree *node = root;
		SkipEmpty ();
		if (*buffer == L_VAR_SPLITTER) buffer++;
		else if (*buffer != L_ARGS_END_SIGN) printf("Syntax error: expected \"%c\" after argument in function\"Def %s\"\n", L_VAR_SPLITTER, functions[free_func - 1].name);
		SkipEmpty ();
		
		while (*buffer != L_ARGS_END_SIGN && *buffer != '\0')
		{
			if (mode == 'D') node->left = OPER(VARLIST_NUM, NULL, GetVar ());
			else node->left = OPER(VARLIST_NUM, NULL, GetE ());
			SkipEmpty ();
			if (*buffer == L_VAR_SPLITTER) buffer++;
			else if (*buffer != L_ARGS_END_SIGN)
			{
				printf("Syntax error: expected \"%c\" after argument in function\"Def %s\"\n", L_VAR_SPLITTER, functions[free_func - 1].name);
				return NULL;
			}
			SkipEmpty ();
			node = node->left;
		}
	}

	if (*buffer == L_ARGS_END_SIGN) buffer++;
	else printf ("Error: expected \"%c\" after \"Def %s\" function declaration", L_ARGS_END_SIGN, functions[free_func - 1].name);
	return root;
}


tree *GetB ()
{
	tree *node = NULL;
	if (L_START_NUM == GetId ('S'))
	{
		buffer++;
		SkipEmpty ();
		node = FUNC(B_NUM, GetOp ());
		buffer++;
		SkipEmpty ();
	}
	else printf("\nSyntax error: expected \"%s\" received %c | %s\n", L_START_, *buffer, buffer);
	return node;
}


tree *GetOp ()
{
	if (*buffer != L_END_SIGN)
	{
		double val = GetId ();
		tree *right = NULL;

		switch ((int) val)
		{
			case L_IF_NUM:
				right = GetIf ();
				break;

			case L_WHILE_NUM:
				right = GetWhile ();
				break;

			case L_VAR_NUM:
				right = GetVar ();
				break;

			case L_RETURN_NUM:
				SkipWord ();
				SkipEmpty ();
				if (*buffer != L_OPER_SPLITTER) right = FUNC(RETURN_NUM, GetE());
				else right = FUNC(RETURN_NUM, NULL);
				break;

			case L_PRINT_NUM:
				SkipWord ();
				SkipEmpty ();
				right = FUNC(PRINT_NUM, GetE());
				break;

			case L_READ_NUM:
				SkipWord ();
				SkipEmpty ();
				right = FUNC(READ_NUM, GetE());
				break;

			default:
				if (FindVar (val) >= 0) right = GetAssn ();
				else if (FindFunc (val) >= 0) right = GetCall ();
				else
				{
					printf ("\nSyntax error: \"%d\" was not declared in this scope\n", val);	
					return NULL;
				}
				break;
		}		
		if (*buffer == L_OPER_SPLITTER) buffer++;
		else if (*buffer == L_END_SIGN) return OPER(OPER_NUM, NULL, right);
		else printf("\nSyntax error: expected '%c' recieved '%c' | %s\n", L_OPER_SPLITTER, *buffer, buffer);
		SkipEmpty ();
		return OPER(OPER_NUM, GetOp (), right);
	}
	else return NULL;
}


tree *GetIf ()
{
	SkipWord ();
	SkipEmpty ();
	tree *condition = GetE ();
	SkipEmpty ();
	return OPER(IF_NUM, condition, GetC ());
}


tree *GetC ()
{
	tree *left = NULL;
	tree *right = GetB ();
	SkipEmpty ();
	if (GetId () == L_ELSE_NUM)
	{
		SkipWord ();
		SkipEmpty ();
		left = GetB ();
		SkipEmpty ();
	}
	return OPER(C_NUM, left, right);
}


tree *GetWhile ()
{
	SkipWord ();
	SkipEmpty ();
	tree *condition = GetE ();
	SkipEmpty ();
	return OPER(WHILE_NUM, condition, GetB ());
}


tree *GetAssn ()
{
	double name = GetId ('S');
	if (FindVar (name) < 0) printf ("\nError: \"%g\" was not declared in this scope\n", name);
	SkipEmpty ();
	if (L_ASSN_NUM == GetId ('S'))
	{
		SkipEmpty ();
		return OPER(ASSN_NUM, VAR(name), GetE ());

	}
	else
	{
		printf("\nSyntax error: expected \"%s\" received %c\n", L_ASSN_, *buffer);
		return NULL;
	}
}


tree *GetCall ()
{
	double name = GetId ('S');
	SkipEmpty ();
	tree *args = GetVarList ();
	return OPER(CALL_NUM, args, FUNC(name, NULL));
}


tree *GetE ()
{
	tree *val = GetS ();
	SkipEmpty ();
	switch ((int) GetId ())
	{
		case L_EQAL_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(EQAL_NUM, val, GetS());
			break;

		case L_NO_EQAL_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(NO_EQAL_NUM, val, GetS());
			break;

		case L_ABOVE_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(ABOVE_NUM, val, GetS());
			break;

		case L_ABOVE_EQ_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(ABOVE_EQ_NUM, val, GetS());
			break;

		case L_BELOW_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(BELOW_NUM, val, GetS());
			break;

		case L_BELOW_EQ_NUM:
			SkipWord ();
			SkipEmpty ();
			val = OPER(BELOW_EQ_NUM, val, GetS());
			break;
	}
	SkipEmpty ();
	return val;
}


tree *GetS ()
{
	tree *val = GetT ();
	SkipEmpty ();
	double oper = GetId ();
	while (oper == L_PLUS_NUM || oper == L_MINUS_NUM)
	{
		SkipWord ();
		SkipEmpty ();
		tree *val2 = GetT ();

		if (oper == L_PLUS_NUM) val = PLUS(val, val2);
		else val = MINUS(val, val2);
		oper = GetId ();
	}
	return val;
}


tree *GetT ()
{
	tree *val = GetPow ();
	SkipEmpty ();
	double oper = GetId ();
	while (oper == L_MUL_NUM || oper == L_DIV_NUM)
	{
		SkipWord ();
		SkipEmpty ();
		tree *val2 = GetPow ();

		if (oper == L_MUL_NUM) val = MUL(val, val2);
		else val = DIV(val, val2);
		oper = GetId ();
	}
	return val;
}


tree *GetPow ()
{
	tree *val = GetP ();
	SkipEmpty ();
	double oper = GetId ();
	while (oper == L_POW_NUM)
	{
		SkipWord ();
		SkipEmpty ();
		val = POW(val, GetP ());
		oper = GetId ();
	}
	return val;
}


tree *GetP ()
{
	tree *val = NULL;
	double oper = GetId ();
	if (oper == L_ARGS_START_NUM)
	{
		SkipWord ();
		SkipEmpty ();
		val = GetE ();
		oper = GetId ('S');
		if (oper != L_CLOSE_NUM) printf("\nSyntax error: expected \"%s\" received %c\n", L_CLOSE_, *buffer);
		SkipEmpty ();
		return val;
	}
	else
	{
		if ('0' <= *buffer && *buffer <= '9' || *buffer == '-') val = GetN ();
		else
		{
			val = GetV ();
			if (val == NULL) val = GetF ();
			if (val == NULL) printf("Syntax error: \"<Empty>\" was not declared in this scope\n");
		}
		return val;
	}
		
}


tree *GetF ()
{
	double val = GetId ();
	if (FindFunc (val) >= 0)
	{
		return GetCall ();
	}
	else if (val == SIN_NUM
	#define FUNCTION(name, diff_description, print_description) || val == name##NUM
	#include "DSL\DSL_function_descriptions.h"
	#undef FUNCTION
	)
	{
		SkipWord ();
		SkipEmpty ();
		return FUNC(val, GetP ());
	}
	else
	{
		printf("Syntax error: unknown function %d\n", val);
		return NULL;
	}
}


tree *GetN ()
{
	double val = 0;
	int sign = 1;
	if (*buffer == '-')
	{
		sign = -1;
		buffer++;
	}
	while ('0' <= *buffer && *buffer <= '9')
	{
		val = val * 10 + (*buffer - '0');
		buffer++;
	}
	double divider = 10;
	if (*buffer == '.')
	{
		buffer++;
		while ('0' <= *buffer && *buffer <= '9')
		{
			val += (*buffer - '0') / divider;
			divider *= 10;
			buffer++;
		}
	}
	val *= sign;
	return NUM(val);
}


tree *GetV ()
{
	double val = GetId ();
	if (FindVar (val) >= 0) SkipWord ();
	else return NULL;
	return VAR(val);
}