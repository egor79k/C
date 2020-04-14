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


int main ()
{
	HT HshTb_1 (input_file, Const_1);
	HshTb_1.Print_lists_length (stat_file);

	HT HshTb_2 (input_file, String_len);
	HshTb_2.Print_lists_length (stat_file);

	HT HshTb_3 (input_file, ASCII_sum);
	HshTb_3.Print_lists_length (stat_file);
	
	return 0;
}