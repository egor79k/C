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
	#define XX 0x80
	const __m128i pixels_01_extend   = _mm_setr_epi8(0,  XX, 1,  XX, 2,  XX, 3,  XX, 4,  XX, 5,  XX, 6,  XX, 7,  XX);
	const __m128i pixels_23_extend   = _mm_setr_epi8(8,  XX, 9,  XX, 10, XX, 11, XX, 12, XX, 13, XX, 14, XX, 15, XX);
	const __m128i alphas_extend      = _mm_setr_epi8(6,  XX, 6,  XX, 6,  XX, 6,  XX, 14, XX, 14, XX, 14, XX, 14, XX);
	const __m128i pixels_01_compress = _mm_setr_epi8(1,  3,  5,  XX, 9,  11, 13, XX, XX, XX, XX, XX, XX, XX, XX, XX);
	const __m128i pixels_23_compress = _mm_setr_epi8(XX, XX, XX, XX, XX, XX, XX, XX, 1,  3,  5,  XX, 9,  11, 13, XX);
	#undef XX

	for (unsigned int str = 0; str < Foreground->Header.Height; str++)
	{
		for (unsigned int cln = 0; cln < Foreground->Header.Width - Foreground->Header.Width % 4; cln += 4)
		{
			// Using formula: ((FC - BC) * FA) / 255 + BC where FC(BC) is Foreground(Background) color

			__m128i Bg_0123 = _mm_load_si128((__m128i *) (&Image[BG_OFFSET]));
			__m128i Fg_0123 = _mm_load_si128((__m128i *) (&Foreground->Image[FG_OFFSET]));

			//====================================================================
			// Bg_0123:  |B0 G0 R0 A0|B1 G1 R1 A1|B2 G2 R2 A2|B3 G3 R3 A3|___  e
			//                                                               | x
			// Bg_01:    |B0 -- G0 -- R0 -- A0 --|B1 -- G1 -- R1 -- A1 --|<--| t
			//                                                               | e
			// Bg_23:    |B2 -- G2 -- R2 -- A2 --|B3 -- G3 -- R3 -- A3 --|<--' n
            //                                                                 d
			__m128i Bg_01 = _mm_shuffle_epi8(Bg_0123, pixels_01_extend);
			__m128i Bg_23 = _mm_shuffle_epi8(Bg_0123, pixels_23_extend);

			//====================================================================
			// Fg_0123:  |B0 G0 R0 A0|B1 G1 R1 A1|B2 G2 R2 A2|B3 G3 R3 A3|___  e
			//                                                               | x
			// Fg_01:    |B0 -- G0 -- R0 -- A0 --|B1 -- G1 -- R1 -- A1 --|<--| t
			//                                                               | e
			// Fg_23:    |B2 -- G2 -- R2 -- A2 --|B3 -- G3 -- R3 -- A3 --|<--' n
			//                                                                 d
			__m128i Fg_01 = _mm_shuffle_epi8(Fg_0123, pixels_01_extend);
			__m128i Fg_23 = _mm_shuffle_epi8(Fg_0123, pixels_23_extend);

			//====================================================================
			// name:     | SB0   SG0   SR0   SA0 | SB1   SG1   SR1   SA1 |
			// Sub_01:   |B0-B0 G0-G0 R0-R0 A0-A0|B1-B1 G1-G1 R1-R1 A1-A1|
			//            /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ 
			//            Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg 
			//
			// name:     | SB2   SG2   SR2   SA2 | SB3   SG3   SR3   SA3 |
			// Sub_23:   |B2-B2 G2-G2 R2-R2 A2-A2|B3-B3 G3-G3 R3-R3 A3-A3|
			//            /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ 
			//            Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg Fg Bg 
            //                                                            
			__m128i Fg_sub_Bg_01 = _mm_sub_epi16(Fg_01, Bg_01);
			__m128i Fg_sub_Bg_23 = _mm_sub_epi16(Fg_23, Bg_23);

			//====================================================================
			// Fg_01:    |B0 -- G0 -- R0 -- A0 --|B1 -- G1 -- R1 -- A1 --|___
			//                                                               |
			// Alpha_01: |A0 -- A0 -- A0 -- A0 --|A1 -- A1 -- A1 -- A1 --|<--'
			//
			// Fg_23:    |B2 -- G2 -- R2 -- A2 --|B3 -- G3 -- R3 -- A3 --|___
			//                                                               |
			// Alpha_23: |A2 -- A2 -- A2 -- A2 --|A3 -- A3 -- A3 -- A3 --|<--'
			//                                                               
			__m128i Alpha_01 = _mm_shuffle_epi8(Fg_01, alphas_extend);
			__m128i Alpha_23 = _mm_shuffle_epi8(Fg_23, alphas_extend);
			
			//====================================================================
			// Mul_01: |SB0*A0 SG0*A0 SR0*A0 SA0*A0|SB1*A1 SG1*A1 SR1*A1 SA1*A1|
			//              /\    /\      /\     /\     /\     /\     /\     /\
			//              Fg     Fg     Fg     Fg     Fg     Fg     Fg     Fg    
			//
			// Mul_23: |SB0*A2 SG0*A2 SR0*A2 SA0*A2|SB1*A3 SG1*A3 SR1*A3 SA1*A3|
			//              /\    /\      /\     /\     /\     /\     /\     /\
			//              Fg     Fg     Fg     Fg     Fg     Fg     Fg     Fg    
			//
			Fg_sub_Bg_01 = _mm_mullo_epi16(Fg_sub_Bg_01, Alpha_01);
			Fg_sub_Bg_23 = _mm_mullo_epi16(Fg_sub_Bg_23, Alpha_23);

			//====================================================================
			// Fg_01:    |B0 -- G0 -- R0 -- A0 --|B1 -- G1 -- R1 -- A1 --|___  c
			//                                                               | o
			// Res_01:   |B0 G0 R0 --|B1 G1 R1 --|-- -- -- --|-- -- -- --|<--' m
			//                                                                 p
			// Fg_23:    |B2 -- G2 -- R2 -- A2 --|B3 -- G3 -- R3 -- A3 --|___  r
			//                                                               | e
			// Res_23:   |-- -- -- --|-- -- -- --|B2 G2 R2 --|B3 G3 R3 --|<--' s
			//                                                                 s
			__m128i Result_01 = _mm_shuffle_epi8(Fg_sub_Bg_01, pixels_01_compress);
			__m128i Result_23 = _mm_shuffle_epi8(Fg_sub_Bg_23, pixels_23_compress);

			//====================================================================
			// Res_01:   |B0 G0 R0 --|B1 G1 R1 --|-- -- -- --|-- -- -- --|___
			//                                                               | a
			// Res_23:   |-- -- -- --|-- -- -- --|B2 G2 R2 --|B3 G3 R3 --|___| d
			//                                                               | d
			// Res_0123: |B0 G0 R0 --|B1 G1 R1 --|B2 G2 R2 --|B3 G3 R3 --|<--'
			//
			__m128i Result_0123 = _mm_add_epi8(Result_01, Result_23);

			//====================================================================
			// Res_0123: |B0 G0 R0 --|B1 G1 R1 --|B2 G2 R2 --|B3 G3 R3 --|___
			//                                                               | a
			// Bg_0123:  |B0 G0 R0 A0|B1 G1 R1 A1|B2 G2 R2 A2|B3 G3 R3 A3|___| d
			//                                                               | d
			// Res_0123: |B0 G0 R0 A0|B1 G1 R1 A1|B2 G2 R2 A2|B3 G3 R3 A3|<--'
			//
			Result_0123 = _mm_add_epi8(Result_0123, Bg_0123);

			_mm_store_si128((__m128i *) (&Image[BG_OFFSET]), Result_0123);
		}
	}

	return;
}

#undef FG_OFFSET
#undef BG_OFFSET