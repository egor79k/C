  //:::::::::::://
 // Hash Table //
//:::::::::::://
#include "List.cpp"


unsigned int Hash_std (char *string);


class HT
{
private:
	//static const unsigned int size = 811;
	//List Hash_table[size] = {};
	unsigned int size = 0;
	List *Hash_table = 0;
	unsigned int (*Hash) (char *);


public:
	HT (int ht_size, unsigned int (*Hash_func) (char *) = Hash_std);
	~HT ();

	void Insert (char *string);
	bool Delete (char *string);
	char *Find (char *string);
	void Print_lists_length (const char *file_name);
};


/**
*	Default hash function for HT class
*
*	@param[in] string String pointer
*
*	return String hash
*/
unsigned int Hash_std (char *string)
{
	const int HXV = 1111111111;
	unsigned int hash = 0;
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
HT::HT (int ht_size, unsigned int (*Hash_func) (char *))
{
	Hash = Hash_func;
	size = ht_size;
	Hash_table = new List[size];
}


/**
*	Hash table destructor
*
*	@param[in] Hash_func Hash function pointer
*/
HT::~HT ()
{
	delete[] Hash_table;
}


/**
*	Insert element
*
*	@param[in] string String pointer
*/
void HT::Insert (char *string)
{
	unsigned int hash = Hash (string) % size;
	Hash_table[hash].InsertAfter (Hash_table[hash].End (), string);
	return;
}


/**
*	Delete element
*
*	@param[in] string String pointer
*
*	return true - success delete | false - no such element
*/
bool HT::Delete (char *string)
{
	return Hash_table[Hash (string) % size].Delete (string);
}


/**
*	Find element
*
*	@param[in] string String pointer
*
*	return Pointer to finded element | NULL - No such element
*/
char *HT::Find (char *string)
{
	unsigned int hash = Hash (string) % size;
	return Hash_table[hash].Find (string);
}


/**
*	Print size of each list in file .csv format
*
*	@param[in] file_name File to print into
*/
void HT::Print_lists_length (const char *file_name)
{
	FILE *output = fopen (file_name, "a");
	for (int i = 0; i < size; ++i)
	{
		fprintf(output, "%d;", Hash_table[i].End ());
	}
	fprintf(output, "\n");
	fclose (output);
	return;
}