   /////////////////////////////////////////
  /// Blend 32-bit BMP images functions ///
 /// For BMP_class.h                   ///
/////////////////////////////////////////
#include <immintrin.h>

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


#define BG_OFFSET ((str_offset + str) * Header.Width + cln_offset + cln) << 2
#define FG_OFFSET (str * (Foreground->Header.Width - Foreground->Header.Width % 4) + cln) << 2

void BMP::Blend_V2  (BMP *Foreground, const uint32_t cln_offset, const uint32_t str_offset)
{
	const __m128i low_pixel_line_half  = _mm_setr_epi8(0, 0x80, 1, 0x80, 2, 0x80, 3, 0x80, 4, 0x80, 5, 0x80, 6, 0x80, 7, 0x80);
	const __m128i high_pixel_line_half = _mm_setr_epi8(8, 0x80, 9, 0x80, 10, 0x80, 11, 0x80, 12, 0x80, 13, 0x80, 14, 0x80, 15, 0x80);
	const __m128i alpha_mask           = _mm_setr_epi8(6, 0x80, 6, 0x80, 6, 0x80, 6, 0x80, 14, 0x80, 14, 0x80, 14, 0x80, 14, 0x80);
	const __m128i store_low_half       = _mm_setr_epi8(1, 3, 5, 0x80, 9, 11, 13, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
	const __m128i store_high_half      = _mm_setr_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 1, 3, 5, 0x80, 9, 11, 13, 0x80);

	for (unsigned int str = 0; str < Foreground->Header.Height; str++)
	{
		for (unsigned int cln = 0; cln < Foreground->Header.Width - Foreground->Header.Width % 4; cln += 4)
		{
			//printf("[%d, %d]\n", str, cln);
			//unsigned int bkg_pos = ((str_offset + str) * Header.Width + cln_offset + cln) << 2;
			//unsigned int frg_pos = (str * (Foreground->Header.Width - Foreground->Header.Width % 4) + cln) << 2;
			//*(uint64_t *)(bkg_ptr + bkg_pos) = *(uint64_t *)(frg_ptr + frg_pos);
			//*(uint64_t *)(bkg_ptr + bkg_pos + 8) = *(uint64_t *)(frg_ptr + frg_pos + 8);

			// Background: |A3|R3|G3|B3| |A2|R2|G2|B2| |A1|R1|G1|B1| |A0|R0|G0|B0|
			// Foreground: |A3|R3|G3|B3| |A2|R2|G2|B2| |A1|R1|G1|B1| |A0|R0|G0|B0|

			__m128i bkg = _mm_load_si128(reinterpret_cast<const __m128i *>(&Image[BG_OFFSET]));
			__m128i frg = _mm_load_si128(reinterpret_cast<const __m128i *>(&Foreground->Image[FG_OFFSET]));

			__m128i bkg1 = _mm_shuffle_epi8(bkg, low_pixel_line_half);
			__m128i bkg2 = _mm_shuffle_epi8(bkg, high_pixel_line_half);

			__m128i frg1 = _mm_shuffle_epi8(frg, low_pixel_line_half);
			__m128i frg2 = _mm_shuffle_epi8(frg, high_pixel_line_half);


			// Diff 1: |__A1|__R1| |__G1|__B1| |__A0|__R0| |__G0|__B0|
			// Diff 2: |__A3|__R3| |__G3|__B3| |__A2|__R2| |__G2|__B2|

			__m128i diff1 = _mm_sub_epi16(frg1, bkg1);
			__m128i diff2 = _mm_sub_epi16(frg2, bkg2);


			// Prepare alphas
			// Alpha 1: |__A1|__A1| |__A1|__A1| |__A0|__A0| |__A0|__A0|
			// Alpha 2: |__A3|__A3| |__A3|__A3| |__A2|__A2| |__A2|__A2|

			__m128i alpha1 = _mm_shuffle_epi8(frg1, alpha_mask);
			__m128i alpha2 = _mm_shuffle_epi8(frg2, alpha_mask);
			

			// Multiply alphas

			diff1 = _mm_mullo_epi16(diff1, alpha1);
			diff2 = _mm_mullo_epi16(diff2, alpha2);


			// Exctract result bytes from diffs

			__m128i res1 = _mm_shuffle_epi8(diff1, store_low_half);
			__m128i res2 = _mm_shuffle_epi8(diff2, store_high_half);
			__m128i result = _mm_add_epi8(res1, res2);
			result = _mm_add_epi8(result, bkg);


			// Store

			_mm_store_si128(reinterpret_cast<__m128i *>(&Image[BG_OFFSET]), result);

//            unsigned char alpha = foreground.image[frg_pos + 3];
//            unsigned char dif2 = (frg_ptr[frg_pos + 2] - bkg_ptr[bkg_pos + 2]);
//            unsigned char dif1 = (frg_ptr[frg_pos + 1] - bkg_ptr[bkg_pos + 1]);
//            unsigned char dif = (frg_ptr[frg_pos + 0] - bkg_ptr[bkg_pos + 0]);
//            bkg_ptr[bkg_pos + 2] = (bkg_ptr[bkg_pos + 2]) + ((dif2 * alpha) >> 8);
//            bkg_ptr[bkg_pos + 1] = (bkg_ptr[bkg_pos + 1]) + ((dif1 * alpha) >> 8);
//            bkg_ptr[bkg_pos] = (bkg_ptr[bkg_pos]) + ((dif * alpha) >> 8);
		}
	}

	return;
}

#undef FG_OFFSET
#undef BG_OFFSET