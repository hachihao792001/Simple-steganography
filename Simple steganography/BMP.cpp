#include "BMP.h"

bool ReadBMP(char* fileName, BMPFile& output) {
	FILE* f;
	fopen_s(&f, fileName, "rb");
	if (f) {
		char BM[2];
		fread(&BM, 2, 1, f);
		if (BM[0] != 'B' || BM[1] != 'M')
			return false;
		fread(&output.header, sizeof(output.header), 1, f);
		fread(&output.dib, sizeof(output.dib), 1, f);
		//fread(&output.colorTable, sizeof(output.colorTable), 1, f);

		int widthSize = output.dib.width * (output.dib.pixelDepth / 8);
		PixelArray pa = output.pixelArray;
		pa.paddingSize = (4 - (widthSize % 4)) % 4;
		pa.lineSize = widthSize + pa.paddingSize;
		pa.rawByteSize = pa.lineSize * output.dib.height;
		pa.rawBytes = new unsigned char[pa.rawByteSize];

		fseek(f, output.header.pixelArrayLocation, SEEK_SET);
		fread(pa.rawBytes, pa.rawByteSize, 1, f);

		pa.rows = output.dib.height;
		pa.cols = output.dib.width;

		pa.pixels = new Color * [pa.rows];
		for (int i = 0; i < (int)pa.rows; i++)
			pa.pixels[i] = (Color*)(pa.rawBytes + pa.lineSize * (pa.rows - 1 - i));

		output.pixelArray = pa;

		fclose(f);
	}
	else {
		cout << "Loi file" << endl;
		return false;
	}

	return true;
}

void WriteBMP(char* fileName, BMPFile input) {
	FILE* f;
	fopen_s(&f, (const char*)fileName, "wb");
	if (f) {
		char BM[2];
		BM[0] = 'B';	BM[1] = 'M';
		fwrite(&BM, 2, 1, f);
		fwrite(&input.header, sizeof(input.header), 1, f);
		fwrite(&input.dib, sizeof(input.dib), 1, f);

		//fwrite(&input.colorTable, sizeof(input.colorTable), 1, f);
		fseek(f, input.header.pixelArrayLocation, SEEK_SET);
		fwrite(input.pixelArray.rawBytes, input.pixelArray.rawByteSize, 1, f);

		fclose(f);
	}
	else {
		cout << "Loi file" << endl;
	}
}