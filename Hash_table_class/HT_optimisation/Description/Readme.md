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
