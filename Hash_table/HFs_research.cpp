#include "Hash_table.cpp"

const char *stat_file = "Statistics.csv";
const char *input_file = "input.txt";
const int HT_SIZE = 811;


/**
*	Constant 1 hash
*
*	@param[in] string String pointer
*
*	return 1
*/
unsigned int Const_1 (char *string)
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
unsigned int String_len (char *string)
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
unsigned int ASCII_sum (char *string)
{
	unsigned int hash = 0;
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
unsigned int ASCII_div_len (char *string)
{
	unsigned int hash = 0;
	unsigned int len = strlen (string);
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
unsigned int Cycle_hash (char *string)
{
	unsigned int hash = 0;
	while (*string != '\0')
	{
		hash = hash xor (unsigned int) *string;
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
unsigned int Jerkins (char *string)
{
	unsigned int hash = 0;
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
	return hash;
}


/**
*	FNV hash function v.1a
*
*	@param[in] string String pointer
*
*	return String hash
*/
unsigned int FNV (char *string)
{
	const unsigned int FNV_32_PRIME = 0x01000193;
	unsigned int hash = 0x811c9dc5;

	while (*string)
	{
		hash ^= *string++;
		hash *= FNV_32_PRIME;
	}

	return hash;
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


void Fill_and_print_HT (unsigned int (*Hash_func) (char *))
{
	HT HshTb;
	HT_Construct (&HshTb, HT_SIZE, Hash_func);
	char *buffer = GetBuffer (input_file);
	char *tmp = buffer;

	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			*buffer = '\0';
			HT_Insert (&HshTb, tmp);
			tmp = buffer + 1;
		}
		buffer++;
	}

	Print_lists_length (&HshTb, stat_file);

	/*printf ("bakery - %s\nsaucepan - %s\nfly - %s\n(null) - %s\n", HT_Find (&HshTb, "bakery"), HT_Find (&HshTb, "saucepan"), HT_Find (&HshTb, "fly"), HT_Find (&HshTb, "krgsp"));
	
	printf ("Delete:%d: ", HT_Delete (&HshTb, "bakery"));
	printf("bakery - %s\n", HT_Find (&HshTb, "bakery"));*/
	HT_Destruct (&HshTb);
	return;
}


int main ()
{
	printf("Constant 1 hashing...\n");
	Fill_and_print_HT (Const_1);

	printf("String length hashing...\n");
	Fill_and_print_HT (String_len);

	printf("ASCII num hashing...\n");
	Fill_and_print_HT (ASCII_sum);

	printf("ASCII num divided by length hashing...\n");
	Fill_and_print_HT (ASCII_div_len);

	printf("Cycle hashing...\n");
	Fill_and_print_HT (Cycle_hash);

	printf("Jerkins hashing...\n");
	Fill_and_print_HT (Jerkins);

	printf("FNV hashing...\n");
	Fill_and_print_HT (FNV);
	
	return 0;
}