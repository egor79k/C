#include "Hash_table.cpp"

const char *stat_file = "Statistics.csv";
/**
*	Xor hash
*
*	@param[in] string String pointer
*
*	return String hash
*/
int Xor_hash (char *string)
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
*	Xor with ASCII and 1-bit shift left
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


int main ()
{
	HT HshTb_1 (input_file, Const_1);
	HshTb_1.Print_lists_length (stat_file);

	HT HshTb_2 (input_file, String_len);
	HshTb_2.Print_lists_length (stat_file);

	HT HshTb_3 (input_file, ASCII_sum);
	HshTb_3.Print_lists_length (stat_file);

	HT HshTb_4 (input_file, ASCII_div_len);
	HshTb_4.Print_lists_length (stat_file);

	HT HshTb_5 (input_file, Cycle_hash);
	HshTb_5.Print_lists_length (stat_file);

	HT HshTb_6 (input_file, Jerkins);
	HshTb_6.Print_lists_length (stat_file);
	
	return 0;
}