#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tree.h"
#include "DSL/DSL_definitions.h"
#include "Common.cpp"

#define TRNS_ERROR "\e[1;35mFrontend: \e[31merror:\e[0m"
#define SKIP_EMPTY while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r') str++;
#define SKIP_WORD while (('a' <= *str && *str <= 'z') || ('A' <= *str && *str <= 'Z') || *str == '_') str++;

char *GetFileName (const char mode[3]);
char *GetBuffer (char file_name[MAX_FILE_NAME]);
int FindVar (int hash);
int FindFunc (int hash);

double GetId (const char mode = 'N');
tree *GetProg ();
tree *GetD ();
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


int main (int argc, const char **argv)
{
	if (argc < 3)
	{
		printf("%s no input or/and output file\n", TRNS_ERROR);
		return 1;
	}
	printf("Frontend called for file \"%s\" -o \"%s\"\n", argv[1], argv[2]);

	str = GetBuffer (argv[1]);

	if (str == NULL)
	{
		printf("%s Wrong input file.\n", TRNS_ERROR);
		return 1;
	}

	printf("|Operating...\n");
	tree *root = GetProg ();
	printf("|Printing...\n");
	PrintTree (root, argv[2]);
	printf("|Dumping...\n");
	Dump (root, "Program/frontend_tree.svg");
	return 0;
}


char *GetFileName (const char mode[3])
{
	bool check_result = false;
	char *file_name = (char *) calloc (MAX_FILE_NAME, sizeof (char));
	char *start = file_name;
	strcpy (file_name, "Program/");
	file_name += 8;
	while (check_result == false)
	{
		scanf ("%s", file_name);
		FILE *file = fopen (start, mode);
		if (file == 0) printf ("|\n|Wrong name of file \"%s\". Please enter correct: ", file_name);
		else check_result = true;
		fclose (file);
	}
	return start;
}


double GetId (const char mode)
{
	double val = 0;
	double multiplier = 1;
	int i = 0;
	while (('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z') || str[i] == '_')
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
		case 'S':
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


tree *GetProg ()
{
	SKIP_EMPTY
	tree *node = GetD ();
	tree *root = node;
	while (*str != '\0')
	{
		node->left = GetD ();
		node = node->left;
	}
	return FUNC(P_NUM, root);
}


tree *GetD ()
{
	//SKIP_EMPTY
	//printf("Symbol: '%c', '%c', '%c', '%c',\n", *str, *(str + 1), *(str + 2), *(str + 3));
	double val = GetId ();
	//printf("GetD: %s\n", str);
	switch ((int) val)
	{
		case L_DEF_NUM:
			return D(GetDef ());
			break;

		case L_VAR_NUM:
			return D(GetVar ());
			break;

		default:
			printf ("\n%s Undefined declaration \"%g\"\n", TRNS_ERROR, val);
			return NULL;
			break;
	}
}


tree *GetDef ()
{
	SKIP_WORD
	SKIP_EMPTY
	double name = GetId ('F');
	SKIP_EMPTY
	tree *args = GetVarList ('D');
	SKIP_EMPTY
	return OPER(DEF_NUM, args, FUNC(name, GetB()));
}


tree * GetVar ()
{
	SKIP_WORD
	SKIP_EMPTY
	double name = GetId ('V');
	SKIP_EMPTY
	if (L_ASSN_NUM == GetId ())
	{
		SKIP_WORD
		SKIP_EMPTY
		return OPER(VAR_NUM, GetE (), VAR(name));
	}
	else return OPER(VAR_NUM, NULL, VAR(name));
}


tree *GetVarList (char mode)
{
	if (L_ARGS_START_NUM != (int) GetId ('S')) printf("Error: expected \"%s\" after \"Def %s\" function declaration", L_ARGS_START_, functions[free_func - 1].name);
	SKIP_EMPTY
	tree *root = NULL;
	if (*str != L_ARGS_END_SIGN)
	{
		if (mode == 'D') root = OPER(VARLIST_NUM, NULL, GetVar ());
		else root = OPER(VARLIST_NUM, NULL, GetE ());
		tree *node = root;
		SKIP_EMPTY
		if (*str == L_VAR_SPLITTER) str++;
		else if (*str != L_ARGS_END_SIGN) printf("Syntax error: expected \"%c\" after argument in function\"Def %s\"\n", L_VAR_SPLITTER, functions[free_func - 1].name);
		SKIP_EMPTY
		
		while (*str != L_ARGS_END_SIGN && *str != '\0')
		{
			if (mode == 'D') node->left = OPER(VARLIST_NUM, NULL, GetVar ());
			else node->left = OPER(VARLIST_NUM, NULL, GetE ());
			SKIP_EMPTY
			if (*str == L_VAR_SPLITTER) str++;
			else if (*str != L_ARGS_END_SIGN)
			{
				printf("Syntax error: expected \"%c\" after argument in function\"Def %s\"\n", L_VAR_SPLITTER, functions[free_func - 1].name);
				return NULL;
			}
			SKIP_EMPTY
			node = node->left;
		}
	}

	if (*str == L_ARGS_END_SIGN) str++;
	else printf ("Error: expected \"%c\" after \"Def %s\" function declaration", L_ARGS_END_SIGN, functions[free_func - 1].name);
	return root;
}


tree *GetB ()
{
	tree *node = NULL;
	if (L_START_NUM == GetId ('S'))
	{
		str++;
		SKIP_EMPTY
		node = FUNC(B_NUM, GetOp ());
		str++;
		SKIP_EMPTY
	}
	else printf("\nSyntax error: expected \"%s\" received %c | %s\n", L_START_, *str, str);
	return node;
}


tree *GetOp ()
{
	if (*str != L_END_SIGN)
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
				SKIP_WORD
				SKIP_EMPTY
				if (*str != L_OPER_SPLITTER) right = FUNC(RETURN_NUM, GetE());
				else right = FUNC(RETURN_NUM, NULL);
				break;

			case L_PRINT_NUM:
				SKIP_WORD
				SKIP_EMPTY
				right = FUNC(PRINT_NUM, GetE());
				break;

			case L_READ_NUM:
				SKIP_WORD
				SKIP_EMPTY
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
		if (*str == L_OPER_SPLITTER) str++;
		else if (*str == L_END_SIGN) return OPER(OPER_NUM, NULL, right);
		else printf("\nSyntax error: expected '%c' recieved '%c' | %s\n", L_OPER_SPLITTER, *str, str);
		SKIP_EMPTY
		return OPER(OPER_NUM, GetOp (), right);
	}
	else return NULL;
}


tree *GetIf ()
{
	SKIP_WORD
	SKIP_EMPTY
	tree *condition = GetE ();
	SKIP_EMPTY
	return OPER(IF_NUM, condition, GetC ());
}


tree *GetC ()
{
	tree *left = NULL;
	tree *right = GetB ();
	SKIP_EMPTY
	if (GetId () == L_ELSE_NUM)
	{
		SKIP_WORD
		SKIP_EMPTY
		left = GetB ();
		SKIP_EMPTY
	}
	return OPER(C_NUM, left, right);
}


tree *GetWhile ()
{
	SKIP_WORD
	SKIP_EMPTY
	tree *condition = GetE ();
	SKIP_EMPTY
	return OPER(WHILE_NUM, condition, GetB ());
}


tree *GetAssn ()
{
	double name = GetId ('S');
	if (FindVar (name) < 0) printf ("\nError: \"%g\" was not declared in this scope\n", name);
	SKIP_EMPTY
	if (L_ASSN_NUM == GetId ('S'))
	{
		SKIP_EMPTY
		return OPER(ASSN_NUM, VAR(name), GetE ());

	}
	else
	{
		printf("\nSyntax error: expected \"%s\" received %c\n", L_ASSN_, *str);
		return NULL;
	}
}


tree *GetCall ()
{
	double name = GetId ('S');
	SKIP_EMPTY
	tree *args = GetVarList ();
	return OPER(CALL_NUM, args, FUNC(name, NULL));
}


tree *GetE ()
{
	tree *val = GetS ();
	SKIP_EMPTY
	switch ((int) GetId ())
	{
		case L_EQAL_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(EQAL_NUM, val, GetS());
			break;

		case L_NO_EQAL_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(NO_EQAL_NUM, val, GetS());
			break;

		case L_ABOVE_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(ABOVE_NUM, val, GetS());
			break;

		case L_ABOVE_EQ_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(ABOVE_EQ_NUM, val, GetS());
			break;

		case L_BELOW_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(BELOW_NUM, val, GetS());
			break;

		case L_BELOW_EQ_NUM:
			SKIP_WORD
			SKIP_EMPTY
			val = OPER(BELOW_EQ_NUM, val, GetS());
			break;
	}
	SKIP_EMPTY
	return val;
}


tree *GetS ()
{
	tree *val = GetT ();
	SKIP_EMPTY
	double oper = GetId ();
	while (oper == L_PLUS_NUM || oper == L_MINUS_NUM)
	{
		SKIP_WORD
		SKIP_EMPTY
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
	SKIP_EMPTY
	double oper = GetId ();
	while (oper == L_MUL_NUM || oper == L_DIV_NUM)
	{
		SKIP_WORD
		SKIP_EMPTY
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
	SKIP_EMPTY
	double oper = GetId ();
	while (oper == L_POW_NUM)
	{
		SKIP_WORD
		SKIP_EMPTY
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
		SKIP_WORD
		SKIP_EMPTY
		val = GetE ();
		oper = GetId ('S');
		if (oper != L_CLOSE_NUM) printf("\nSyntax error: expected \"%s\" received %c\n", L_CLOSE_, *str);
		SKIP_EMPTY
		return val;
	}
	else
	{
		if ('0' <= *str && *str <= '9' || *str == '-') val = GetN ();
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
	#include "DSL/DSL_function_descriptions.h"
	#undef FUNCTION
	)
	{
		SKIP_WORD
		SKIP_EMPTY
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
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		val = val * 10 + (*str - '0');
		str++;
	}
	double divider = 10;
	if (*str == '.')
	{
		str++;
		while ('0' <= *str && *str <= '9')
		{
			val += (*str - '0') / divider;
			divider *= 10;
			str++;
		}
	}
	val *= sign;
	return NUM(val);
}


tree *GetV ()
{
	double val = GetId ();
	if (FindVar (val) >= 0) SKIP_WORD
	else return NULL;
	return VAR(val);
}