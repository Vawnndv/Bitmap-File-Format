#include "Header.h"

int main() {
	BmpFile bmp;
	readBmpFile((char*)BMP_FILE, bmp);
	printBmpHeader(bmp);
	printBmpDib(bmp);
	drawBmp_pixel(bmp);
	getchar();
	system("cls");
	drawBmp(bmp);
	releaseBmpPixelArray(bmp);
}