#include "Hash_table.cpp"

const char *input_file = "input.txt";
const char *output_file = "output.txt";
const int HT_SIZE = 56087;//111127;//380041;//56087;
const int WORDS_NUM = 1152320;//828725;//562433;


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

int count = 0;
void Fill_HT (HT &HshTb, char *buffer)
{
	char *tmp = buffer;
	FILE *output = fopen (output_file, "w");

	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			count++;
			*buffer = '\0';
			if (HshTb.Find (tmp) == NULL)
			{
				fprintf(output, "%s\n", tmp);
				HshTb.Insert (tmp);
			}
			tmp = buffer + 1;
		}
		buffer++;
	}
	printf("%d\n", count);
	fclose (output);
	return;
}


int main ()
{
	HT HshTb (HT_SIZE, MurmurHash);
	char *buffer = GetBuffer (input_file);
	char *buff_start = buffer;

	Fill_HT (HshTb, buffer);
	HshTb.Print_lists_length ("stat.csv");
	//for (int i = 0; i < 1000; ++i)
	//	Find_in_HT (HshTb, buff_start);
/*
	HshTb.Insert ("cheRB");
	printf("\n%s\n", HshTb.Find ("cheRB"));
	HshTb.Delete ("cheRB");
	printf("\n%s\n", HshTb.Find ("cheRB"));
*/
	free (buff_start);
	return 0;
}