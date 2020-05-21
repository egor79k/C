#include "BMP_class.h"

const char *Foreground_file = "AskhatCat.bmp";
const char *Background_file = "Table.bmp";
const char *Result_file = "Result.bmp";


int main ()
{
	BMP Cat;
	Cat.Upload (Foreground_file);
	printf("\n\e[1;21;32m|Cat info:\e[0m\n");
	Cat.ViewImageInfo ();

	BMP Table;
	Table.Upload (Background_file);
	printf("\n\n\e[1;21;32m|Table info:\e[0m\n");
	Table.ViewImageInfo ();

	//for (int i = 0; i < 1000; ++i)
		Table.Blend_V2 (&Cat, 1, 80);

	Table.Unload (Result_file);

	return 0;
}