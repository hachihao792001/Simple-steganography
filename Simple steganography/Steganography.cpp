#include "Steganography.h"

void hideToColorChannelByte(byte& colorChannel, string fromBits, int bitNum, int& hiddenBitNum) {
	string colorChannelBits = decToBin(colorChannel);
	if (hiddenBitNum < bitNum) {
		colorChannelBits[6] = fromBits[hiddenBitNum++];
		colorChannelBits[7] = fromBits[hiddenBitNum++];
	}
	colorChannel = binToDec(colorChannelBits);
}

void hide(byte* fromBytes, int byteNum, BMPFile& toBMP) {

	string fromBits = bitset<32>(byteNum).to_string();	//để kích thước file được giấu ở 4 byte đầu
	for (int i = 0; i < byteNum; i++)
		fromBits += decToBin(fromBytes[i]);
	int bitNum = byteNum * 8;

	int b = 0;
	int hiddenBitNum = 0;
	bool reachEnd = false;
	for (int i = toBMP.dib.height - 1; i >= 0; i--) {
		for (int j = 0; j < toBMP.dib.width; j++) {
			hideToColorChannelByte(toBMP.pixelArray.pixels[i][j].blue, fromBits, bitNum, hiddenBitNum);
			if (hiddenBitNum >= bitNum)
				return;
			hideToColorChannelByte(toBMP.pixelArray.pixels[i][j].green, fromBits, bitNum, hiddenBitNum);
			if (hiddenBitNum >= bitNum)
				return;
			hideToColorChannelByte(toBMP.pixelArray.pixels[i][j].red, fromBits, bitNum, hiddenBitNum);
			if (hiddenBitNum >= bitNum)
				return;

			*(toBMP.pixelArray.rawBytes + (b++)) = toBMP.pixelArray.pixels[i][j].blue;
			*(toBMP.pixelArray.rawBytes + (b++)) = toBMP.pixelArray.pixels[i][j].green;
			*(toBMP.pixelArray.rawBytes + (b++)) = toBMP.pixelArray.pixels[i][j].red;

		}
		for (int j = 0; j < toBMP.pixelArray.paddingSize; j++)
			*(toBMP.pixelArray.rawBytes + (b++)) = (byte)0;
	}
}

bool extractFromColorChannelByte(byte colorChannel, string& current8Bits, string& resultBytesStr, int& extractedBitNum, int& hiddenByteNum, string& hiddenByteNumStr) {
	string colorChannelBits = decToBin(colorChannel);
	current8Bits += colorChannelBits[6];
	current8Bits += colorChannelBits[7];
	extractedBitNum += 2;
	if (current8Bits.length() == 8) {
		if (resultBytesStr.length() == hiddenByteNum)
			return false;	//lấy xong dữ liệu (số byte đã lấy = kích thước file được giấu)

		if (extractedBitNum > 32) {
			//đã lấy xong kích thước file được giấu, giờ bắt đầu lấy các byte dữ liệu
			resultBytesStr += (char)binToDec(current8Bits);
		}
		else {
			//kích thước file được giấu, được để ở 4 byte đầu lúc giấu
			hiddenByteNumStr += current8Bits;
			if (extractedBitNum == 32) 
				hiddenByteNum = bitset<32>(hiddenByteNumStr).to_ulong();
		}
		current8Bits = "";
	}
	return true;
}

int extract(BMPFile fromBMP, byte*& resultBytes) {
	string hiddenByteNumStr = "";
	string resultBytesStr = "";
	string current8Bits = "";
	int hiddenByteNum = -1;
	int extractedBitNum = 0;

	for (int i = fromBMP.dib.height - 1; i >= 0; i--) {
		for (int j = 0; j < fromBMP.dib.width; j++) {
			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].blue, current8Bits, resultBytesStr, extractedBitNum, hiddenByteNum, hiddenByteNumStr)) {
				resultBytes = new byte[hiddenByteNum];
				for (int i = 0; i < hiddenByteNum; i++)
					resultBytes[i] = (byte)resultBytesStr[i];
				return hiddenByteNum;
			}

			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].green, current8Bits, resultBytesStr, extractedBitNum, hiddenByteNum, hiddenByteNumStr)) {
				resultBytes = new byte[hiddenByteNum];
				for (int i = 0; i < hiddenByteNum; i++)
					resultBytes[i] = (byte)resultBytesStr[i];
				return hiddenByteNum;
			}

			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].red, current8Bits, resultBytesStr, extractedBitNum, hiddenByteNum, hiddenByteNumStr)) {
				resultBytes = new byte[hiddenByteNum];
				for (int i = 0; i < hiddenByteNum; i++)
					resultBytes[i] = (byte)resultBytesStr[i];
				return hiddenByteNum;
			}
		}
	}
}