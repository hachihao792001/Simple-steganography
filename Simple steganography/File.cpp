#include "File.h"

int GetFileBytes(string fileName, byte*& result) {
	FILE* file;
	fopen_s(&file, (char*)fileName.c_str(), "rb");
	if (!file)return 0;

	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	result = new byte[fileSize];
	fread(result, 1, fileSize, file);
	fclose(file);
	return fileSize;
}

void CreateFileFromBytes(string fileName, byte* bytes, int byteCount) {
	FILE* file;
	fopen_s(&file, (char*)fileName.c_str(), "wb");
	if (!file) return;

	fwrite(bytes, 1, byteCount, file);
	fclose(file);
}
