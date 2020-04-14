#include "List.cpp"

const int HASH_XOR_VAL = 1111111111;
const char *input_file = "input.txt";
const char *stat_file = "Statistics.csv";

int Hash_std (char *string);


class HT
{
private:
	static const int HT_SIZE = 1979;

	List Hash_table[HT_SIZE] = {};
	int (*Hash) (char *);


public:
	HT (const char *input_file, int (*Hash_func) (char *) = Hash_std);
	~HT () {}
	void Print_lists_length (const char *file_name);
};


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


/**
*	Default hash function for HT class
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Hash_std (char *string)
{
	int hash = 0;
	while (*string != '\0')
	{
		hash += *string;
		hash = hash xor HASH_XOR_VAL;
		string++;
	}
	return hash;
}


HT::HT (const char *input_file, int (*Hash_func) (char *))
{
	Hash = Hash_func;
	char *buffer = GetBuffer (input_file);
	char *tmp = buffer;
	int hash = 0;
	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			*buffer = '\0';
			hash = Hash (tmp) % HT_SIZE;
			Hash_table[hash].InsertAfter (Hash_table[hash].head, tmp);
			tmp = buffer + 1;
		}
		buffer++;
	}
}


/**
*	Hash count
*
*	@param[in] string String pointer
*
*	return String hash
*/
//void Insert ()



/**
*	Print size of each list in file .csv format
*/
void HT::Print_lists_length (const char *file_name)
{
	FILE *output = fopen (file_name, "w");
	for (int i = 0; i < HT_SIZE; ++i)
	{
		fprintf(output, "%d;", Hash_table[i].free_elem - 1);
	}
	fprintf(output, "\n");
	fclose (output);
	return;
}


/**
*	Hash count
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Count_hash (char *string)
{
	int hash = 0;
	while (*string != '\0')
	{
		hash += *string;
		hash = hash xor HASH_XOR_VAL;
		string++;
	}
	return hash;
}


int main ()
{
	HT HshTb (input_file);

	HshTb.Print_lists_length (stat_file);
	
	return 0;
}