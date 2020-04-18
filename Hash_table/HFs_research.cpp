#include "Hash_table.cpp"

const char *stat_file = "Statistics.csv";
const char *input_file = "input.txt";


/**
*	Constant 1 hash
*
*	@param[in] string String pointer
*
*	return 1
*/
int Const_1 (char *string)
{
	return 1;
}


/**
*	String length hash
*
*	@param[in] string String pointer
*
*	return String hash
*/
int String_len (char *string)
{
	return strlen (string);
}


/**
*	Summ of string's ASCII symbols
*
*	@param[in] string String pointer
*
*	return String hash
*/
int ASCII_sum (char *string)
{
	int hash = 0;
	while (*string != '\0')
	{
		hash += *string;
		string++;
	}
	return hash;
}


/**
*	Summ of string's ASCII symbols divided by strlen
*
*	@param[in] string String pointer
*
*	return String hash
*/
int ASCII_div_len (char *string)
{
	int hash = 0;
	int len = strlen (string);
	while (*string != '\0')
	{
		hash += *string;
		string++;
	}
	return hash / len;
}


/**
*	Xor with ASCII and 1-bit shift left
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Cycle_hash (char *string)
{
	int hash = 0;
	while (*string != '\0')
	{
		hash = hash xor (int) *string;
		hash = hash << 1 | hash >> 31;
		string++;
	}
	return hash;
}


/**
*	Jerkins hash function
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Jerkins (char *string)
{
	unsigned short hash = 0;
	while (*string != '\0')
	{
		hash += *string;
		hash += (hash << 10);
		hash ^= (hash >> 6);
		string++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return (int) hash;
}


char *GetBuffer (const char *file_name)
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


void Fill_and_print_HT (int (*Hash_func) (char *))
{
	HT HshTb (Hash_func);
	char *buffer = GetBuffer (input_file);
	char *tmp = buffer;
	int hash = 0;

	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			*buffer = '\0';
			HshTb.Insert (tmp);
			tmp = buffer + 1;
		}
		buffer++;
	}

	HshTb.Print_lists_length (stat_file);
/*
	printf ("bakery - %s\nsaucepan - %s\nfly - %s\n(null) - %s\n", HshTb.Find ("bakery"), HshTb.Find ("saucepan"), HshTb.Find ("fly"), HshTb.Find ("krgsp"));
	
	printf ("Delete:%d: ", HshTb.Delete ("bakery"));
	printf("bakery - %s\n", HshTb.Find ("bakery"));*/
	return;
}


int main ()
{
	Fill_and_print_HT (Const_1);

	Fill_and_print_HT (String_len);

	Fill_and_print_HT (ASCII_sum);

	Fill_and_print_HT (ASCII_div_len);

	Fill_and_print_HT (Cycle_hash);

	Fill_and_print_HT (Jerkins);
	
	return 0;
}