#include <iostream>


const char *Input_file = "AskhatCat.bmp";


struct BitMapHeader
{
	uint16_t Type            = 0;
	uint32_t FileSize        = 0;
	uint16_t Reserved1       = 0;
	uint16_t Reserved2       = 0;
	uint32_t OffsetBits      = 0;
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
	BitMapHeader Header;

public:
	BMP (const char *FileName);
	~BMP ();

	void ViewImageInfo ();

};


BMP::BMP (const char *FileName)
{
	FILE *input = fopen (FileName, "rb");

	if (input == NULL) printf("Error: Can't open file \"%s\"\n", FileName);
	else
	{
		fread (&Header, 1, 0x36, input);
		//fread (Image, 1, 14, input);
		fclose (input);
	}
}


BMP::~BMP ()
{

}


void BMP::ViewImageInfo ()
{
	printf(	"Type            = %x\n"
			"FileSize        = %x\n"
			"Reserved1       = %x\n"
			"Reserved2       = %x\n"
			"OffsetBits      = %x\n"
			"InfoHeaderSize  = %x\n"
			"Width           = %x\n"
			"Height          = %x\n"
			"Planes          = %x\n"
			"BitCount        = %x\n"
			"Compression     = %x\n"
			"SizeImage       = %x\n"
			"XpxlsPerMeter   = %x\n"
			"YpxlsPerMeter   = %x\n"
			"ColorsUsed      = %x\n"
			"ColorsImportant = %x\n",
			//*((char *) &Header.Type),
			//*(((char *) &Header.Type) + 1),
			Header.Type,
			Header.FileSize,
			Header.Reserved1,
			Header.Reserved2,
			Header.OffsetBits,
			Header.InfoHeaderSize,
			Header.Width,
			Header.Height,
			Header.Planes,
			Header.BitCount,
			Header.Compression,
			Header.SizeImage,
			Header.XpxlsPerMeter,
			Header.YpxlsPerMeter,
			Header.ColorsUsed,
			Header.ColorsImportant);

	return;
}


int main ()
{
	BMP Cat (Input_file);

	Cat.ViewImageInfo ();

	return 0;
}