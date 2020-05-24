


const char ASM_BEGIN[] = 	"global _start		\n\n"
							"section .bss		\n\t"
							"loc_mem resb 16384	\n\n"
							"section .text		\n"
							"_start:			\n"
							"	xor r8, r8		\n";


const char ASM_IN[]   = 	"	xor rax, rax	; Read\n"
							"	mov rdx, 1		\n"
							"	mov rdi, 1		\n"
							"	syscall			\n";


const char ASM_OUT[]  =		"	pop qword [loc_mem]		; Print\n"
							"	mov rsi, loc_mem		\n"
							"	mov rdx, 1				\n"
							"	mov rdi, 1				\n"
							"	mov rax, 1				\n"
							"	syscall					\n";


const char ASM_SQRT[] =		"	pop rbx					; Root\n"
							"	mov rax, rbx			\n"
							"	xor rcx, rcx			\n"
							"	.sqrt_%d:				\n"
							"		cmp rbx, 0			\n"
							"		jbe .sqrt_int_%d	\n"
							"		mov rdx, rcx		\n"
							"		shl rdx, 1			\n"
							"		inc rdx				\n"
							"		sub rbx, rdx		\n"
							"		inc rcx				\n"
							"		cmp rbx, -2147480000\n"
							"		jb .sqrt_%d 		\n"
							"	mov rbx, rcx			\n"
							"	imul rbx, rbx			\n"
							"	add rax, rbx			\n"
							"	shl rcx, 1				\n"
							"	xor rdx, rdx			\n"
							"	div rcx					\n"
							"	mov rcx, rax			\n"
							"	.sqrt_int_%d:			\n"
							"	push rcx				\n";


const char ASM_RET_VAL[] =	"	pop rbx     ; Save up stack val	\n"
							"	pop rax     ; Save ret addr		\n"
							"	push rbx    ; Push up stack val	\n"
							"	push rax    ; Push ret addr		\n";


const char A_[]   = "<Empty>";
const char PUSH[] = "push";
const char POP[]  = "pop";
const char ADD[]  = "add";
const char SUB[]  = "sub";
const char MUL[]  = "mul";
const char DIV[]  = "div";

/*const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";*/

/*add rax, 48
	mov [loc_mem], rax
	mov rsi, loc_mem
	mov rdx, 1
	mov rdi, 1
	mov rax, 1
	syscall*/