#include "Header.h"

bool isBmpFile(FILE* f) {
	BmpSignature signature;
	fseek(f, 0, SEEK_SET);
	fread(&signature, sizeof(BmpSignature), 1, f);
	if (signature.data[0] == 'B' && signature.data[1] == 'M')
		return 1;
	else
		return 0;
}

void readBmpHeader(FILE* f, BmpFile& bmp) {
	fseek(f, 0, SEEK_SET);
	fread(&bmp.header, sizeof(BmpHeader), 1, f);
}

void readBmpDib(FILE* f, BmpFile& bmp) {
	fseek(f, sizeof(BmpHeader), SEEK_SET);
	fread(&bmp.dib, sizeof(BmpDib), 1, f);
}

void printBmpHeader(BmpFile bmp) {
	cout <<"--------------- BMP HEADER ---------------\n";
	cout <<"- File Size  : " << bmp.header.fileSize << " byte(s) \n";
	cout <<"- Reserved1  : " << bmp.header.reserved1 << endl;
	cout <<"- Reserved2  : " << bmp.header.reserved2 << endl;
	cout <<"- Data Offset: " << bmp.header.dataOffset << " byte(s) \n";
}

void printBmpDib(BmpFile bmp) {
	cout <<"----------------- BMP DIB -----------------\n";
	cout <<"- DIB Size               : " << bmp.dib.dibSize << " byte(s) \n";
	cout <<"- Image Width            : " << bmp.dib.imageWidth << endl;
	cout <<"- Image Height           : " << bmp.dib.imageHeight << endl;
	cout <<"- Number of Color Planes : " << bmp.dib.colorPlaneCount << endl;
	cout <<"- Pixel Size             : " << bmp.dib.pixelSize << " bit(s) \n";
	cout <<"- Compress Method        : " << bmp.dib.compressMethod << endl;
	cout <<"- Bitmap Size            : " << bmp.dib.bitmapByteCount << " byte(s) \n";
	cout <<"- Horizontal Resolution  : " << bmp.dib.horizontalResolution << endl;
	cout <<"- Vertical Resolution    : " << bmp.dib.verticalResolution << endl;
	cout <<"- Number of Colors       : " << bmp.dib.colorCount << endl;
	cout <<"- Number of Impt Colors  : " << bmp.dib.importantColorCount << endl;
	cout <<"-------------------------------------------\n";
}

void readBmpPixelArray(FILE* f, BmpFile& bmp) {
	int widthSize = bmp.dib.imageWidth * (bmp.dib.pixelSize / 8);
	bmp.pixelArray.paddingSize = (4 - (widthSize % 4)) % 4;
	bmp.pixelArray.lineSize = widthSize + bmp.pixelArray.paddingSize;
	bmp.pixelArray.rawByteSize = bmp.pixelArray.lineSize * bmp.dib.imageHeight;
	bmp.pixelArray.rawBytes = new unsigned char[bmp.pixelArray.rawByteSize];

	fseek(f, bmp.header.dataOffset, SEEK_SET);
	fread(bmp.pixelArray.rawBytes, bmp.pixelArray.rawByteSize, 1, f);

	initPixels(bmp.pixelArray, bmp.dib.imageWidth, bmp.dib.imageHeight);
}

void initPixels(PixelArray& pixelarray, int width, int height) {
	pixelarray.rowCount = height;
	pixelarray.columnCount = width;

	pixelarray.pixels = new Color * [pixelarray.rowCount];
	for (int i = 0; i < pixelarray.rowCount; i++)
		pixelarray.pixels[pixelarray.rowCount - 1 - i] = (Color*)(pixelarray.rawBytes + pixelarray.lineSize * i);
}

void drawBmp(BmpFile bmp) {
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < bmp.dib.imageHeight; i++)
		for (int j = 0; j < bmp.dib.imageWidth; j++) {
			Color pixel = bmp.pixelArray.pixels[i][j];
			SetPixel(hdc, j, i, RGB(pixel.red, pixel.green, pixel.blue));
		}

	ReleaseDC(console, hdc);
}

void drawBmp_pixel(BmpFile bmp) {
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	int i, j;
	cout << "Enter i: "; cin >> i;
	cout << "Enter j: "; cin >> j;
	cout << "Red		: " <<(int)bmp.pixelArray.pixels[i]->red<< endl;
	cout << "Green		: " <<(int)bmp.pixelArray.pixels[i]->green << endl;
	cout << "Blue		: " <<(int)bmp.pixelArray.pixels[i]->blue << endl;
	Color pixel = bmp.pixelArray.pixels[i][j];
	SetPixel(hdc, j, i, RGB(pixel.red, pixel.green, pixel.blue));
	ReleaseDC(console, hdc);
	getchar();
}

void releaseBmpPixelArray(BmpFile& bmp) {
	delete[]bmp.pixelArray.rawBytes;
	delete[]bmp.pixelArray.pixels;
}

void readBmpFile(char* filename, BmpFile& bmp) {
	FILE* f = fopen(filename, "rb");

	if (f == NULL || !isBmpFile(f)) {
		cout <<"Error or Invalid Bmp File.\n";
		return;
	}
	readBmpHeader(f, bmp);
	readBmpDib(f, bmp);
	readBmpPixelArray(f, bmp);
	fclose(f);
}