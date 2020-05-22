Alpha blending
==============
This program blends two BMP-32 images: ovelays foreground picture with transparency on background. Each pixel is 4 bytes and includes |B|G|R|A|. Alpha blend naive formula:

	Bg[B] = (Fg[A] * Fg[B] + (255 - Fg[A]) * Bg[B]) / 255;
	Bg[G] = (Fg[A] * Fg[G] + (255 - Fg[A]) * Bg[G]) / 255;
	Bg[R] = (Fg[A] * Fg[R] + (255 - Fg[A]) * Bg[R]) / 255;

3,92x speed up
--------------
It takes 5 minutes 6 seconds to do 30000 blendings.
After intrinsic SSE optimization this time reduced to 1 minute 18 seconds.
As a result the speed up coefficient is 306/78 around 3,92x.

Example
-------
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Murzik.bmp "Cat")
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Mouse.bmp  "Mouse")

Result
------
![Image alt](https://github.com/egor79k/C/blob/master/AlphaBlend/Result.bmp "Result")
