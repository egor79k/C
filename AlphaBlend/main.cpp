#include <iostream>


struct BitMapFileHeader
{
	uint16_t Type       = 0;
	uint32_t FileSize   = 0;
	uint16_t Reserved1  = 0;
	uint16_t Reserved2  = 0;
	uint32_t OffsetBits = 0;
};

struct BitMapInfoHeader
{
	uint32_t InfoHeaderSize  = 0;
	uint32_t Width           = 0;
	uint32_t Height          = 0;
	uint16_t Planes          = 0;
	uint16_t BitCount        = 0;
	uint32_t Compression     = 0;
	uint32_t SizeImage       = 0;
	uint32_t XpxlsPerMeter   = 0;
	uint32_t YpxlsPerMeter   = 0;
	uint32_t ColorsUsed      = 0;
	uint32_t ColorsImportant = 0;
};


class BMP
{
private:
	char *Image;
	BitMapFileHeader FileHeader;
	BitMapInfoHeader InfoHeader;

public:
	BMP (char *FileName);
	~BMP ();

	ImageInfo ();

};


BMP::BMP (cahr *FileName);
{
	FILE *input = fopen (FileName, "rb");
	if (input != NULL) printf("Error: Can't open file \"%s\"\n", FileName);
	else
	{
		fread (&FileHeader, 1, 14, input);
		fread (&InfoHeader, 1, 14, input);
		//fread (Image, 1, 14, input);
		fclose (input);
	}
}


BMP::~BMP ()
{

}


BMP::ImageInfo ()
{
	printf("%c%c\n", char Type, );
}


int main ()
{


	return 0;
}