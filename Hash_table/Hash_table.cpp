  //:::::::::::://
 // Hash Table //
//:::::::::::://
#include "List.cpp"


int Hash_std (char *string);


struct HT
{
	List *Hash_table;
	int size;
	int (*Hash) (char *);
};

	bool HT_Construct (HT *HshTb, int size, int (*Hash_func) (char *) = Hash_std);
	void HT_Destruct (HT *HshTb);

	void HT_Insert (char *string);
	bool HT_Delete (char *string);
	char *HT_Find (char *string);
	void Print_lists_length (const char *file_name);


/**
*	Default hash function for HT class
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Hash_std (char *string)
{
	const int HXV = 1111111111;
	int hash = 0;
	while (*string != '\0')
	{
		hash += *string;
		hash = hash xor HXV;
		string++;
	}
	return hash;
}


/**
*	Hash table constructor
*
*	@param[in] Hash_func Hash function pointer
*/
bool HT_Construct (HT *HshTb, int size, int (*Hash_func) (char *))
{
	HshTb->size = size;
	HshTb->Hash = Hash_func;
	HshTb->Hash_table = (List *) calloc (HshTb->size, sizeof (List));
	for (int i = 0; i < HshTb->size; ++i) ListConstruct (&HshTb->Hash_table[i]);
	return true;
}


/**
*	Hash table destructor
*
*	@param[in] Hash_func Hash function pointer
*/
void HT_Destruct (HT *HshTb)
{
	for (int i = 0; i < HshTb->size; ++i) ListDestruct (&HshTb->Hash_table[i]);
	free (HshTb->Hash_table);
	return;
}


/**
*	Insert element
*
*	@param[in] string String pointer
*/
void HT_Insert (HT *HshTb, char *string)
{
	int hash = HshTb->Hash (string) % HshTb->size;
	InsertAfter (&HshTb->Hash_table[hash], End (&HshTb->Hash_table[hash]), string);
	return;
}


/**
*	Delete element
*
*	@param[in] string String pointer
*
*	return true - success delete | false - no such element
*/
bool HT_Delete (HT *HshTb, char *string)
{
	int hash = HshTb->Hash (string) % HshTb->size;
	return Delete (&HshTb->Hash_table[hash], string);
}


/**
*	Find element
*
*	@param[in] string String pointer
*
*	return Pointer to finded element | NULL - No such element
*/
char *HT_Find (HT *HshTb, char *string)
{
	int hash = HshTb->Hash (string) % HshTb->size;
	return Find (&HshTb->Hash_table[hash], string);
}


/**
*	Print size of each list in file .csv format
*
*	@param[in] file_name File to print into
*/
void Print_lists_length (HT *HshTb, const char *file_name)
{
	FILE *output = fopen (file_name, "a");
	for (int i = 0; i < HshTb->size; ++i)
	{
		fprintf(output, "%d;", End (&HshTb->Hash_table[i]));
	}
	fprintf(output, "\n");
	fclose (output);
	return;
}