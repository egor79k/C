HASH TABLE OPTIMIZATION
=======================
1.Standart
----------
The original version of Hash Table.

![Image alt](https://github.com/egor79k/programms/blob/master/Images/1_Standart.png "Standart")
![Image alt](https://github.com/egor79k/programms/blob/master/Images/1_Standart_diag.png "Standart diagramm")

|Optimization|Time (sec.)|
|:----------:|:---------:|
|-O0|30|
|-O1|24,2|
|-O3|16,9|

2.Strcmp ASM optimization
-------------------------
Change standard strcmp() function on inline assembler code. Reduced time for Find by 4%. We have around 26,4 seconds now.

![Image alt](https://github.com/egor79k/programms/blob/master/Images/2_Strcmp_opt.png "Asm optimization")

![Image alt](https://github.com/egor79k/programms/blob/master/Images/2_Strcmp_func_1.png "Asm optimization")

3.Strcmp SSE optimization
-------------------------
Optimize Find function with inline SSE compare instructions. Current time is around 22 seconds.

![Image alt](https://github.com/egor79k/programms/blob/master/Images/3_Strcmp_opt.png "SSE optimization")

![Image alt](https://github.com/egor79k/programms/blob/master/Images/3_Strcmp_func_1.png "SSE optimization")

|Optimization|Time (sec.)|
|:----------:|:---------:|
|ASM|26,4|
|+SSE|22,4|

4.Hash asm optimization
-----------------------
Optimize MurMurHash v.2A function with inline assembler. Full time is around 18 seconds now. -O optimizations don’t give speed up here. MurMur is in the “green zone” at present.

```c++
    //===============================
		// Input: RSI - 1-st string
		//        RCX - string length
		// Outpt: RAX - hash
		// Destr: RBX RDX RDI
		//===============================

		asm (".intel_syntax noprefix				\n"

			"	mov		edi, ecx		\n" // unsigned int len = l;
			"	mov		eax, 42764812		\n" // unsigned int h = MUR_SEED;
			
			".loc_loop:					\n" // while (len >= 4)
			"	cmp		ecx, 4			\n"
			"	jb		.stop			\n"
			"	mov		ebx, dword ptr [rsi]	\n" // k = *(unsigned int*)string;

			"	imul    ebx, 0x5BD1E995 		\n" // mmix (h, k);
			"	mov     edx, ebx			\n"
			"	shr     edx, 24				\n"
			"	xor     ebx, edx			\n"
			"	imul    ebx, 0x5BD1E995 		\n"
			"	imul	eax, 0x5BD1E995 		\n"
			"	xor		eax, ebx		\n"

			"	add rsi, 4				\n"	// string += 4;
			"	sub ecx, 4				\n"	// len -= 4;

			"	jmp .loc_loop				\n"

			"	.stop:					\n"

			"	xor		ebx, ebx		\n" // unsigned int t = 0;

			"	cmp		ecx, 3			\n" // switch (len)
			"	je		.length_3 		\n"
			"	cmp		ecx, 2			\n"
			"	je		.length_2 		\n"
			"	cmp		ecx, 1			\n"
			"	je		.length_1 		\n"
			"	jmp		.length_0 		\n"

			".length_3:					\n" // case 3: t ^= string[2] << 16;
			"	xor		edx, edx		\n"
			"	mov		dl, byte ptr [rsi + 2]	\n"
			"	shl		edx, 16			\n"
			"	xor		ebx, edx		\n"

			".length_2:					\n" // case 2: t ^= string[1] << 8;
			"	xor		edx, edx		\n"
			"	mov		dl, byte ptr [rsi + 1]	\n"
			"	shl		edx, 8			\n"
			"	xor		ebx, edx		\n"

			".length_1:					\n" // case 1: t ^= string[0];
			"	xor		edx, edx		\n"
			"	mov		dl, byte ptr [rsi]	\n"
			"	xor		ebx, edx		\n"

			".length_0:					\n"

			"	imul    ebx, 0x5BD1E995 		\n" // mmix (h, t);
			"	mov     edx, ebx			\n"
			"	shr     edx, 24				\n"
			"	xor     ebx, edx			\n"
			"	imul    ebx, 0x5BD1E995 		\n"
			"	imul	eax, 0x5BD1E995 		\n"
			"	xor		eax, ebx		\n"

			"	imul    edi, 0x5BD1E995 		\n" // mmix (h, l);
			"	mov     edx, edi			\n"
			"	shr     edx, 24				\n"
			"	xor     edi, edx			\n"
			"	imul    edi, 0x5BD1E995 		\n"
			"	imul	eax, 0x5BD1E995 		\n"
			"	xor		eax, edi		\n"

			"	mov     edx, eax			\n" // h ^= h >> 13;
			"	shr     edx, 13				\n"
			"	xor     eax, edx			\n"
 
			"	imul    eax, 0x5BD1E995			\n" // h *= m;
 
			"	mov     edx, eax			\n" // h ^= h >> 15;
			"	shr     edx, 15				\n"
			"	xor     eax, edx			\n"

			".att_syntax prefix				\n"
			
			: "=a" (hash)
			: "S" (string), "c" (strlen (string))
			: "rbx", "rdx", "rdi"
			);
```
![Image alt](https://github.com/egor79k/programms/blob/master/Images/4_MurMur_opt.png "Asm optimization")

5.Result statistics
-------------------
|Optimization|Time (sec.)|Coefficient|
|:----------:|:---------:|:---------:|
|-O0|30|1|
|-O1|24,2|1,24|
|-O3|16,9|1,78|
|STRCMP ASM|26,4|1,14|
|STRCMP SSE|22,4|1,34|
|HASH ASM|18|1,67|


![Image alt](https://github.com/egor79k/programms/blob/master/Images/5_Diag.png "SSE optimization")
