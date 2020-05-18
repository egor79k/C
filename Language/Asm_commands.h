


const char ASM_BEGIN[] = 	"global _start		\n\n"
							"section .bss		\n\t"
							"loc_mem resb 1024	\n\n"
							"section .text		\n"
							"_start:			\n\t"
							"	xor rdi, rdi	\n";

const char PUSH[] = "push";
const char POP[]  = "pop";
const char ADD[]  = "add";
const char SUB[]  = "sub";
const char MUL[]  = "mul";
const char DIV[]  = "div";

const char IN[]   = "\t xor rax, rax	\n\t"
					"	mov rdx, 1		\n\t"
					"	mov rdi, 1		\n\t"
					"	mov rsi, loc_mem\n\t"
					"	syscall			\n\t"
					"	push qword [loc_mem]\n";

const char OUT[]  = "OUT";
/*const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";
const char [] = "";*/
