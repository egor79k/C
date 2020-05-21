#include <iostream>


const char *Foreground_file = "AskhatCat.bmp";
const char *Background_file = "Table.bmp";
const char *Result_file = "Result.bmp";
const int Header_size = 0x36;


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
	BitMapHeader Header;
	char *Image;

public:
	BMP () {}
	~BMP ();

	bool Upload (const char *FileName);
	bool Unload (const char *FileName);
	bool Blend  (BMP *Foreground);
	void ViewImageInfo ();

};


BMP::~BMP ()
{
	free (Image);
}


bool BMP::Upload (const char *FileName)
{
	char *temp_header = (char *) calloc (Header_size, sizeof (char));
	char *temp_header_start = temp_header;
	FILE *input = fopen (FileName, "rb");

	if (input == NULL)
	{
		printf("Error: Can't open file \"%s\"\n", FileName);
		return false;
	}

	fread (temp_header, sizeof (char), Header_size, input);

	Header.Type            = *((uint16_t *) temp_header);
	temp_header += sizeof (uint16_t);
	Header.FileSize        = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.Reserved1       = *((uint16_t *) temp_header);
	temp_header += sizeof (uint16_t);
	Header.Reserved2       = *((uint16_t *) temp_header);
	temp_header += sizeof (uint16_t);
	Header.OffsetBits      = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.InfoHeaderSize  = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.Width           = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.Height          = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.Planes          = *((uint16_t *) temp_header);
	temp_header += sizeof (uint16_t);
	Header.BitCount        = *((uint16_t *) temp_header);
	temp_header += sizeof (uint16_t);
	Header.Compression     = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.SizeImage       = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.XpxlsPerMeter   = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.YpxlsPerMeter   = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.ColorsUsed      = *((uint32_t *) temp_header);
	temp_header += sizeof (uint32_t);
	Header.ColorsImportant = *((uint32_t *) temp_header);

	free (temp_header_start);

	Image = (char *) calloc (Header.SizeImage, sizeof (char));

	fread (Image, sizeof (char), Header.SizeImage, input);

	fclose (input);
	
	return true;
}


bool BMP::Unload (const char *FileName)
{
	char *temp_header = (char *) calloc (Header_size, sizeof (char));
	char *temp_header_start = temp_header;

	*((uint16_t *) temp_header) = Header.Type;
	temp_header += sizeof (uint16_t);
	*((uint32_t *) temp_header) = Header.FileSize;
	temp_header += sizeof (uint32_t);
	*((uint16_t *) temp_header) = Header.Reserved1;
	temp_header += sizeof (uint16_t);
	*((uint16_t *) temp_header) = Header.Reserved2;
	temp_header += sizeof (uint16_t);
	*((uint32_t *) temp_header) = Header.OffsetBits;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.InfoHeaderSize;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.Width;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.Height;
	temp_header += sizeof (uint32_t);
	*((uint16_t *) temp_header) = Header.Planes;
	temp_header += sizeof (uint16_t);
	*((uint16_t *) temp_header) = Header.BitCount;
	temp_header += sizeof (uint16_t);
	*((uint32_t *) temp_header) = Header.Compression;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.SizeImage;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.XpxlsPerMeter;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.YpxlsPerMeter;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.ColorsUsed;
	temp_header += sizeof (uint32_t);
	*((uint32_t *) temp_header) = Header.ColorsImportant;

	FILE *output = fopen (FileName, "wb");

	fwrite (temp_header_start, sizeof (char), Header_size, output);
	fwrite (Image, sizeof (char), Header.SizeImage, output);

	free (temp_header_start);
	fclose (output);
	
	return true;
}


void BMP::ViewImageInfo ()
{
	printf(	"Type            = %c%c\n"
			"FileSize        = %d\n"
			"Reserved1       = %d\n"
			"Reserved2       = %d\n"
			"OffsetBits      = %d\n"
			"InfoHeaderSize  = %d\n"
			"Width           = %d\n"
			"Height          = %d\n"
			"Planes          = %d\n"
			"BitCount        = %d\n"
			"Compression     = %d\n"
			"SizeImage       = %d\n"
			"XpxlsPerMeter   = %d\n"
			"YpxlsPerMeter   = %d\n"
			"ColorsUsed      = %d\n"
			"ColorsImportant = %d\n",
			*((char *) &Header.Type),
			*(((char *) &Header.Type) + 1),
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


#define B_B Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2)]
#define B_G Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 1]
#define B_R Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 2]
#define B_A (Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 3] & 0xff)

#define F_B Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2)]
#define F_G Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 1]
#define F_R Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 2]
#define F_A (Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 3] & 0xff)

bool BMP::Blend  (BMP *Foreground)
{
	const uint32_t str_offset = 250;
	const uint32_t cln_offset = 200;
	uint32_t str = 0; 
	uint32_t cln = 0;

	while (str < Foreground->Header.Height)
	{
		while (cln < Foreground->Header.Width)
		{
			B_B = (F_A * F_B + (255 - F_A) * B_B) / 255;
			B_G = (F_A * F_G + (255 - F_A) * B_G) / 255;
			B_R = (F_A * F_R + (255 - F_A) * B_R) / 255;
			//printf("[%x %x] ", B_A, F_A);
			cln++;
		}

		cln = 0;
		str++;
	}

	return true;
}

#undef B_B
#undef B_G
#undef B_R
#undef B_A

#undef F_B
#undef F_G
#undef F_R
#undef F_A


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

	Table.Blend (&Cat);

	Table.Unload (Result_file);

	return 0;
}