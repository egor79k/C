  //:::::::::::://
 // Hash Table //
//:::::::::::://
#include "List.cpp"


int Hash_std (char *string);


class HT
{
private:
	static const int HT_SIZE = 811;
	List Hash_table[HT_SIZE] = {};
	int (*Hash) (char *);


public:
	HT (int (*Hash_func) (char *) = Hash_std);
	~HT () {}

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
HT::HT (int (*Hash_func) (char *))
{
	Hash = Hash_func;
}


/**
*	Insert element
*
*	@param[in] string String pointer
*/
void HT::Insert (char *string)
{
	int hash = Hash (string) % HT_SIZE;
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
	return Hash_table[Hash (string) % HT_SIZE].Delete (string);
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
	int hash = Hash (string) % HT_SIZE;
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
	for (int i = 0; i < HT_SIZE; ++i)
	{
		fprintf(output, "%d;", Hash_table[i].End ());
	}
	fprintf(output, "\n");
	fclose (output);
	return;
}