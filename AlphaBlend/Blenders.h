   /////////////////////////////////////////
  /// Blend 32-bit BMP images functions ///
 /// For BMP_class.h                   ///
/////////////////////////////////////////

#define B_B Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2)]
#define B_G Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 1]
#define B_R Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 2]
#define B_A (Image[((((str + str_offset) * Header.Width) + cln + cln_offset) << 2) + 3] & 0xff)

#define F_B Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2)]
#define F_G Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 1]
#define F_R Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 2]
#define F_A (Foreground->Image[(((str * Foreground->Header.Width) + cln) << 2) + 3] & 0xff)

void BMP::Blend_V1  (BMP *Foreground, const uint32_t cln_offset, const uint32_t str_offset)
{
	for (uint32_t str = 0; str < Foreground->Header.Height; ++str)
	{
		for (uint32_t cln = 0; cln < Foreground->Header.Width; ++cln)
		{
			B_B = (F_A * F_B + (255 - F_A) * B_B) / 255;
			B_G = (F_A * F_G + (255 - F_A) * B_G) / 255;
			B_R = (F_A * F_R + (255 - F_A) * B_R) / 255;
		}
	}

	return;
}

#undef B_B
#undef B_G
#undef B_R
#undef B_A

#undef F_B
#undef F_G
#undef F_R
#undef F_A


void BMP::Blend_V2  (BMP *Foreground, const uint32_t cln_offset, const uint32_t str_offset)
{
	for (uint32_t str = 0; str < Foreground->Header.Height; ++str)
	{
		for (uint32_t cln = 0; cln < Foreground->Header.Width; ++cln)
		{
			
		}
	}

	return;
}
