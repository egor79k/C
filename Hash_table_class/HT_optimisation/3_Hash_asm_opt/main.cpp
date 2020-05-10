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
/*	unsigned int hash = 0;

	asm (".intel_syntax noprefix\n"

		"%macro mmix 2			\n"
		"	mul %2, M_VAL		\n"	// mmix(h,k);
		"	mov ebx, eax		\n"
		"	shr ebx, R_VAL		\n"
		"	xor %2, ebx			\n"
		"	mul %2, M_VAL		\n"
		"	mul %1, M_VAL		\n"
		"	xor %1, %2			\n"
		"%endmacro				\n"


		"MUR_SEED equ 42764812  \n"
		"M_VAL    equ 0x5bd1e995\n"
		"R_VAL    equ 24        \n"
		
		"	mov r9, rcx         \n"	// unsigned int len = l;
		"	xor rdx, rdx		\n"
		"	mov edx, MUR_SEED   \n"	// unsigned int h = MUR_SEED;
		"	xor eax, eax        \n"	// unsigned int k = 0;

									
		".loop:                 \n"	// while (len >= 4)
		"	cmp r9, 4           \n"
		"	jb .loop_end       \n"
		"	mov eax, [rsi]      \n"	// k = *(unsigned int*)string;
		
		"	mmix edx, eax\n"
*/
/*		"	mul eax, M_VAL      \n"	// mmix(h,k);
		"	mov ebx, eax        \n"
		"	shr ebx, R_VAL      \n"
		"	xor eax, ebx        \n"
		"	mul eax, M_VAL      \n"
		"	mul edx, M_VAL      \n"
		"	xor edx, eax        \n"*/
/*
		"	add rsi, 4          \n"	// string += 4;
		"	sub r9, 4           \n"	// len -= 4;
		"	jmp .loop           \n"
		".loop_end:"

		"	xor r8d, r8d        \n"	// unsigned int t = 0;
		"	xor rbx, rbx		\n"

		"	cmp r9, 3			\n"
		"	jne .next_3			\n"
		"	mov bl, [rsi + 2]"
		"	shl, rbx, 16		\n"
		"	xor, r8d, ebx		\n"
		".next_3:				\n"

		"	cmp r9, 2			\n"
		"	jne .next_2			\n"
		"	mov bl, [rsi + 1]"
		"	shl, rbx, 8			\n"
		"	xor, r8d, ebx		\n"
		".next_2:				\n"

		"	cmp r9, 1			\n"
		"	jne .next_1			\n"
		"	xor, r8d, [rsi]		\n"
		".next_1:				\n"

		"	mmix edx, r8d		\n"
		"	mmix edx, rcx		\n"

		"	mov ebx, edx		\n"
		"	shr ebx, 13			\n"
		"	xor edx, ebx		\n"
		"	mul edx, M_VAL		\n"
		"	mov ebx, edx		\n"
		"	shr ebx, 15			\n"
		"	xor edx, ebx		\n"

		".att_syntax prefix\n"
			
		: "=d"(hash)
		: "c"(strlen (string)), "S"(string)
		: "rax", "rbx", "r8", "r9"
		);*/

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
		Find_in_HT (HshTb, find_buff_start);


	free (buffer);
	free (find_buff_start);
	return 0;
}