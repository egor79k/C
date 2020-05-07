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
unsigned int Jerkins (char *string)
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
	return (unsigned int) hash;
}


/**
*	MurMurHash v.2A
*
*	@param[in] string String pointer
*
*	return String hash
*/
#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }
const unsigned int MUR_SEED = 42764812;

unsigned int MurmurHash (char *string)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int l = strlen (string);
	unsigned int len = l;
	unsigned int h = MUR_SEED;
	unsigned int k;

	while (len >= 4)
	{
		k = *(unsigned int*)string;

		mmix(h,k);

		string += 4;
		len -= 4;
	}

	unsigned int t = 0;

	switch (len)
	{
	case 3: t ^= string[2] << 16;
	case 2: t ^= string[1] << 8;
	case 1: t ^= string[0];
	};

	mmix(h,t);
	mmix(h,l);

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
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
	HT HshTb (Hash_func);
	char *buffer = GetBuffer (input_file);
	char *tmp = buffer;

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
	printf ("War - %s\nand - %s\nPeace - %s\n(null) - %s\n", HshTb.Find ("War"), HshTb.Find ("and"), HshTb.Find ("Peace"), HshTb.Find ("krgsp"));
	
	printf ("Delete:%d: ", HshTb.Delete ("War"));
	printf("War - %s\n", HshTb.Find ("War"));*/
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

	Fill_and_print_HT (MurmurHash);
	
	return 0;
}