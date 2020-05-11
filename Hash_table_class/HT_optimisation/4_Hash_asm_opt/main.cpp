#include <x86intrin.h>
#include "Hash_table.cpp"

const char *input_file = "input.txt";
const char *input_find_file = "input_repeat.txt";
const int HT_SIZE = 27779;//380041;//56087;
const int WORDS_NUM = 1271839;//1152320;//828725;//562433;


/**
*	MurMurHash v.2A
*
*	@param[in] string String pointer
*
*	return String hash
*/
//#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }
//const unsigned int MUR_SEED = 42764812;

unsigned int MurmurHash (char *string)
{
	unsigned int hash = 0;

	asm (".intel_syntax noprefix\n"

		"	mov     ecx,42764812\n"
        "	xor     ecx,ebx\n"
 
		".loc_loop:		\n"
        "	cmp     ebx,4\n"
        "	jb      .loop_done\n"
 
        "	imul    ecx,0x5BD1E995\n"
 
        "	mov     eax, dword ptr [rsi]\n"
 
        "	imul    eax,0x5BD1E995\n"
        "	mov     edx,eax\n"
        "	shr     edx,24\n"
        "	xor     eax,edx\n"
        "	imul    eax,0x5BD1E995\n"
        "	imul    ecx,0x5BD1E995\n"
        "	xor     ecx,eax\n"
 
        "	add     rsi,4\n"
        "	sub     ebx,4\n"
        "	jmp     .loc_loop\n"
 
		".loop_done:		\n"
        "	cmp     ebx,3\n"
        "	je      .loc_tail_3\n"
        "	cmp     ebx,2\n"
        "	je      .loc_tail_2\n"
        "	cmp     ebx,1\n"
        "	je      .loc_tail_1\n"
        "	jmp     .loc_finish\n"
 
		".loc_tail_3:		\n"
        "	mov     al,byte[rsi+2]\n"
        "	shl     eax,16\n"
        "	xor     ecx,eax\n"
		".loc_tail_2:		\n"
        "	mov     al,byte[rsi+1]\n"
        "	shl     eax,8\n"
        "	xor     ecx,eax\n"
		".loc_tail_1:		\n"
        "	mov     al,byte[rsi]\n"
        "	xor     ecx,eax\n"
        "	imul    ecx,0x5BD1E995\n"
 
		".loc_finish:		\n"
        "	mov     eax,ecx\n"
        "	shr     eax,13\n"
        "	xor     ecx,eax\n"
 
        "	imul    ecx,0x5BD1E995\n"
 
        "	mov     eax,ecx\n"
        "	shr     eax,15\n"
        "	xor     eax,ecx\n"

        ".att_syntax prefix\n"

        : "=a" (hash)
        : "S" (string), "b" (strlen (string))
        : "rcx", "rdx", "rdi"
		);

/*
	asm (".intel_syntax noprefix\n"

		"	mov		edi, ecx			\n"	// unsigned int len = l;
		"	mov		eax, 42764812		\n"	// unsigned int h = MUR_SEED;
		"	mov		ebx, 0x5BD1E995		\n"	// unsigned int k = 0;
 
		".loc_loop:						\n"	// while (len >= 4)
		"	cmp     ecx, 4\n"
		"	jb      .stop\n"

		"	mov		edx, dword [rsi]	\n"	// k = *(unsigned int*)string;
		"	imul	ebx, edx			\n"

		"	imul    ebx, 0x5BD1E995 	\n" // mmix (h, k);
		"	mov     edx, ebx			\n"
		"	shr     edx, 24				\n"
		"	xor     ebx, edx			\n"
		"	imul    ebx, 0x5BD1E995 	\n"
		"	imul	eax, 0x5BD1E995 	\n"
		"	xor		eax, ebx			\n"

		"	add     rsi, 4				\n"	// string += 4;
		"	sub     ecx, 4				\n"	// len -= 4;
		"	jmp     .loc_loop			\n"
 
		".stop:							\n"

		"	xor		ebx, ebx			\n"	// unsigned int t = 0;

		"	cmp     ecx, 3				\n"
		"	je      .length_3			\n"
		"	cmp     ecx, 2				\n"
		"	je      .length_2			\n"
		"	cmp     ecx, 1				\n"
		"	je      .length_1			\n"
		"	jmp		.length_0			\n"

		".length_3:						\n"
		"	mov     dl, byte ptr [rsi + 2]	\n"
		"	shl     edx, 16				\n"
		"	xor     ebx, edx			\n"

		".length_2:						\n"
		"	mov     dl, byte ptr [rsi + 1]	\n"
		"	shl     edx, 8				\n"
		"	xor     ebx, edx			\n"

		".length_1:						\n"
		"	mov		dl, byte ptr [rsi]		\n"
		"	xor     ebx, edx			\n"

		".length_0:						\n"
		"	imul    eax, 0x5BD1E995\n"

		"	imul    ebx, 0x5BD1E995 	\n" // mmix (h, t);
		"	mov     edx, ebx			\n"
		"	shr     edx, 24				\n"
		"	xor     ebx, edx			\n"
		"	imul    ebx, 0x5BD1E995 	\n"
		"	imul	eax, 0x5BD1E995 	\n"
		"	xor		eax, ebx			\n"

		"	imul    edi, 0x5BD1E995 	\n" // mmix (h, l);
		"	mov     edx, edi			\n"
		"	shr     edx, 24				\n"
		"	xor     edi, edx			\n"
		"	imul    edi, 0x5BD1E995 	\n"
		"	imul	eax, 0x5BD1E995 	\n"
		"	xor		eax, edi			\n"

		"	mov     edx, eax\n"
		"	shr     edx, 13\n"
		"	xor     eax, edx\n"
 
		"	imul    eax, 0x5BD1E995\n"
 
		"	mov     edx, eax\n"
		"	shr     edx, 15\n"
		"	xor     eax, edx\n"

		".att_syntax prefix\n"
 
 		: "=a" (hash)
 		: "S" (string), "c" (strlen (string))
		: "rbx", "rdx", "rdi"
		);
*/
	return hash;
}


#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }
const unsigned int MUR_SEED = 42764812;

unsigned int MurmurHash_std (char *string)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int l = strlen (string);
	unsigned int len = l;
	unsigned int h = MUR_SEED;
	unsigned int k = m;

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
			//printf("finding %s...\n", tmp);
			//fflush (stdout);
			//printf("%s\n", HshTb.Find (tmp));
			//fflush (stdout);
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
/*	unsigned int a = 3;
	unsigned int c = 12;
	unsigned int res = 0;

	asm (".intel_syntax noprefix\n"
		"imul rbx, rdx\n"
		"mov rcx, rbx\n"
		
		".att_syntax prefix\n"

		: "=c" (res)
		: "b" (a), "d" (c)
		);

	printf("res = %u\n", res);
*/
	printf("std: %d - %d\n", MurmurHash_std ("started"), MurmurHash ("started"));

	int count = 0;
	HT HshTb (HT_SIZE, MurmurHash);

	char *buffer = GetBuffer (input_file);			//Filling HT with words from input file
	Fill_HT (HshTb, buffer);

	HshTb.Print_lists_length ("stat.csv");

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

	for (int i = 0; i < 1; ++i)					//Finding words in HT
	{
		Find_in_HT (HshTb, find_buff_start);
		//printf("%d ", i);
	}


	free (buffer);
	free (find_buff_start);
	return 0;
}