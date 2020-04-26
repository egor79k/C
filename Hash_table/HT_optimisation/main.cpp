#include "Hash_table.cpp"

const char *input_file = "input.txt";
const int HT_SIZE = 811;


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
	HT HshTb;
	HT_Construct (&HshTb, HT_SIZE, MurmurHash);
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

	/*printf ("bakery - %s\nsaucepan - %s\nfly - %s\n(null) - %s\n", HT_Find (&HshTb, "bakery"), HT_Find (&HshTb, "saucepan"), HT_Find (&HshTb, "fly"), HT_Find (&HshTb, "krgsp"));
	
	printf ("Delete:%d: ", HT_Delete (&HshTb, "bakery"));
	printf("bakery - %s\n", HT_Find (&HshTb, "bakery"));*/
	HT_Destruct (&HshTb);
	return;
	return 0;
}