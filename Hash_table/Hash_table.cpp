#include "List.cpp"

const int HT_SIZE = 19;
const int START_LIST_SIZE = 2;
const int HASH_XOR_VAL = 1111111111;
const char input_file[] = "input.txt";

list Hash_table[HT_SIZE] = {};
char *buffer = {};


int Count_hash (char *string);


int main ()
{
	FILE *input = fopen (input_file, "r");
	fseek (input, 0, SEEK_END);
	int NChars = ftell (input);
	fseek (input, 0, SEEK_SET);
	buffer = (char *) calloc (NChars, sizeof (char));
	fread (buffer, sizeof (char), NChars, input);
	fclose (input);

	int i = 0;

	for (i = 0; i < HT_SIZE; ++i)
	{
		ListConstruct (&Hash_table[i], START_LIST_SIZE);
	}

	i = 0;
	char *tmp = buffer;
	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			*buffer = '\0';
			InsertAfter (&Hash_table[Count_hash (tmp)], Hash_table[Count_hash (tmp)].head, tmp);
			//printf("%15s %d\n", tmp, Count_hash (tmp));
			tmp = buffer + 1;
		}
		buffer++;
	}

	for (i = 0; i < HT_SIZE; ++i)
	{
		//Dump (&Hash_table[i]);
		printf("%d\n", Hash_table[i].free_elem - 1);
	}

	char st[] = "stone";
	int stn = Count_hash (st);
	printf("%d [%p]  %d = %s\n", stn, &Hash_table[stn], FindByValue (&Hash_table[stn], st), Hash_table[stn].data[FindByValue (&Hash_table[stn], st)]);
	return 0;
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
	return hash % HT_SIZE;
}