


const char ASM_BEGIN[] = 	"global _start		\n\n"
							"section .bss		\n\t"
							"loc_mem resb 16384	\n\n"
							"section .text		\n"
							"_start:			\n"
							"	xor r8, r8		\n";

const char PUSH[] = "push";
const char POP[]  = "pop";
const char ADD[]  = "add";
const char SUB[]  = "sub";
const char MUL[]  = "mul";
const char DIV[]  = "div";

const char IN[]   = "	xor rax, rax	; Read\n"
					"	mov rdx, 1		\n"
					"	mov rdi, 1		\n"
					"	syscall			\n";

const char OUT[]  =	"	pop qword [loc_mem]		; Print\n"
					"	mov rsi, loc_mem		\n"
					"	mov rdx, 1				\n"
					"	mov rdi, 1				\n"
					"	mov rax, 1				\n"
					"	syscall					\n";
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