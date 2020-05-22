Alpha blending
==============
This program blends two BMP-32 images: ovelays foreground picture with transparency on background. Each pixel is 4 bytes and includes |B|G|R|A|. Alpha blend formula:

	Bg[B] = (Fg[A] * Fg[B] + (255 - Fg[A]) * Bg[B]) / 255;
	Bg[G] = (Fg[A] * Fg[G] + (255 - Fg[A]) * Bg[G]) / 255;
	Bg[R] = (Fg[A] * Fg[R] + (255 - Fg[A]) * Bg[R]) / 255;

Example
-------
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Murzik.bmp "Cat")
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Mouse.bmp  "Mouse")

Result
------
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Result.bmp "Result")
