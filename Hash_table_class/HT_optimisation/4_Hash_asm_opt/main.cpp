#include <x86intrin.h>
#include "Hash_table.cpp"

const char *input_file = "input.txt";
const char *input_find_file = "input_repeat.txt";
const int HT_SIZE = 27779;//380041;//56087;
const int WORDS_NUM = 1278241;//1152320;//828725;//562433;


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
	unsigned int hash = 0;

	asm (".intel_syntax noprefix\n"

	//	"	mov     rsi,[lpData]\n"
	//	"	mov     ebx,[dSize]\n"
 
		"	mov     ecx, 47264812\n"
		"	xor     ecx, ebx\n"
 
		".loc_loop:		\n"
		"	cmp     ebx, 4\n"
		"	jb      .stop\n"
 
		"	imul    ecx, 0x5BD1E995\n"
 
		"	mov     edx, byte [rsi+1]\n"
		"	mov     eax, byte [rsi]\n"
		"	shl     edx, 8\n"
		"	or      eax, edx\n"
		"	mov     edx, byte [rsi+2]\n"
		"	shl     edx, 16\n"
		"	or      eax, edx\n"
		"	mov     edx, byte [rsi+3]\n"
		"	shl     edx, 24\n"
		"	or      eax, edx\n"
 
		"	imul    eax, 0x5BD1E995\n"
		"	mov     edx, eax\n"
		"	shr     edx, 24\n"
		"	xor     eax, edx\n"
		"	imul    eax, 0x5BD1E995\n"
 
		"	xor     ecx, eax\n"
 
		"	add     rsi, 4\n"
		"	sub     ebx, 4\n"
		"	jmp     .loc_loop\n"
 
		".stop:	\n"

		"	cmp     ebx, 3\n"
		"	jne     .loc_tail_2\n"
		"	mov     eax, byte[rsi+2]\n"
		"	shl     eax, 16\n"
		"	xor     ecx, eax\n"

		".loc_tail_2:		\n"
		"	cmp     ebx, 2\n"
		"	jne     .loc_tail_1\n"
		"	mov     eax, byte[rsi+1]\n"
		"	shl     eax, 8\n"
		"	xor     ecx, eax\n"

		".loc_tail_1:		\n"
		"	mov   eax, byte[rsi]\n"
		"	xor     ecx, eax\n"
		"	imul    ecx, 0x5BD1E995\n"
 
		".loc_finish:		\n"
		"	mov     eax, ecx\n"
		"	shr     eax, 13\n"
		"	xor     ecx, eax\n"
 
		"	imul    ecx, 0x5BD1E995\n"
 
		"	mov     eax, ecx\n"
		"	shr     eax, 15\n"
		"	xor     eax, ecx\n"

		".att_syntax prefix\n"
 
 		: "=a" (hash)
 		: "S" (string), "b" (strlen (string))
		: "rcx", "rdx", "rdi"
		);

/*
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int l = strlen (string);
	unsigned int len = l;
	unsigned int h = MUR_SEED;
	unsigned int k = 0;

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

	return h;*/
	return hash;
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


void Fill_HT (HT &HshTb, char *buffer)
{
	int count = 0;
	char *tmp = buffer;

	while (*buffer != '\0')
	{
		if (*buffer == '\n')
		{
			count++;
			//printf("%d\n", count);
			//fflush (stdout);
			*buffer = '\0';
			HshTb.Insert (tmp);
			tmp = buffer + 1;
		}
		buffer++;
	}
	printf("%d\n", count);
	return;
}


void Find_in_HT (HT &HshTb, char *buffer)
{
	int count = 0;
	char *tmp = buffer;
	while (count < WORDS_NUM)
	{
		if (*buffer == '\0')
		{
			//printf("%s\n", HshTb.Find (tmp));
			HshTb.Find (tmp);
			tmp = buffer + 1;
			count++;
		}
		buffer++;
	}
	//printf("%d ", count);
	return;
}


int main ()
{
	int count = 0;
	HT HshTb (HT_SIZE, MurmurHash);


	char *buffer = GetBuffer (input_file);			//Filling HT with words from input file
	Fill_HT (HshTb, buffer);

	//HshTb.Print_lists_length ("stat.csv");

	char *find_buff = GetBuffer (input_find_file);	//Reading find buffer from file
	char *find_buff_start = find_buff;

	while (count < WORDS_NUM)						//Preparing buffer for find requests
	{
		if (*find_buff == '\n')
		{
			*find_buff = '\0';
			count++;
		}
		find_buff++;
	}

	for (int i = 0; i < 200; ++i)					//Finding words in HT
	{
		Find_in_HT (HshTb, find_buff_start);
		//printf("%d ", i);
	}


	free (buffer);
	free (find_buff_start);
	return 0;
}