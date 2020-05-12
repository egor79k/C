HASH TABLE OPTIMIZATION
=======================
1.Standart
----------
The original version of Hash Table without any optimizations.

![Image alt](https://github.com/egor79k/programms/blob/master/Images/1_Standart.png "Standart")

|Optimization|Time (sec.)|
|:----------:|:---------:|
|-O0|30|
|-O1|24,2|
|-O3|16,9|

![Image alt](https://github.com/egor79k/programms/blob/master/Images/1_Standart_diag.png "Standart")

2.Strcmp ASM optimization
-------------------------
Change standard strcmp() function on inline assembler code. Reduced time for Find by 4%. We have around 26,4 seconds now.

![Image alt](https://github.com/egor79k/programms/blob/master/Images/2_Strcmp_opt.png "Standart")

![Image alt](https://github.com/egor79k/programms/blob/master/Images/2_Strcmp_func_1.png "Standart")
