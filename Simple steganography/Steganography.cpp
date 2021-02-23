#include "Steganography.h"

void hide(byte* fromBytes, int byteNum, BMPFile& toBMP) {
	string fromBits = bitset<32>(byteNum).to_string();	//để kích thước file được giấu ở 4 byte đầu
	for (int i = 0; i < byteNum; i++)
		fromBits += decTo8Bit(fromBytes[i]);

	int numOfBitForHiding = byteNum * 8 + 32;
	int b = 0;
	int hiddenBitNum = 0;

	int height = toBMP.dib.height;
	int width = toBMP.dib.width;

	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			if (hiddenBitNum < numOfBitForHiding) {
				byte* blueByte = toBMP.pixelArray.rawBytes + (b++);
				*blueByte = modifyBit(*blueByte, 1, fromBits[hiddenBitNum++]);
				*blueByte = modifyBit(*blueByte, 0, fromBits[hiddenBitNum++]);
			}
			else
				return;

			if (hiddenBitNum < numOfBitForHiding) {
				byte* greenByte = toBMP.pixelArray.rawBytes + (b++);
				*greenByte = modifyBit(*greenByte, 1, fromBits[hiddenBitNum++]);
				*greenByte = modifyBit(*greenByte, 0, fromBits[hiddenBitNum++]);
			}
			else
				return;

			if (hiddenBitNum < numOfBitForHiding) {
				byte* redByte = toBMP.pixelArray.rawBytes + (b++);
				*redByte = modifyBit(*redByte, 1, fromBits[hiddenBitNum++]);
				*redByte = modifyBit(*redByte, 0, fromBits[hiddenBitNum++]);
			}
			else
				return;
		}
		b += toBMP.pixelArray.paddingSize;
	}
}

bool extractFromColorChannelByte(byte colorChannel, string& resultBits, int& extractedBitNum, int& hiddenByteNum, string& hiddenByteNumBits) {
	if (resultBits.length() >= (size_t)(hiddenByteNum * 8))
		return false;//lấy xong dữ liệu (số byte đã lấy = kích thước file được giấu)

	string colorChannelLast2Bits = "";
	colorChannelLast2Bits += getBitAtPos(colorChannel, 1);
	colorChannelLast2Bits += getBitAtPos(colorChannel, 0);
	extractedBitNum += 2;

	if (extractedBitNum > 32) {
		//đã lấy xong kích thước file được giấu, giờ bắt đầu lấy các byte dữ liệu
		resultBits += colorChannelLast2Bits;
	}
	else {
		//kích thước file được giấu, được để ở 4 byte đầu lúc giấu
		hiddenByteNumBits += colorChannelLast2Bits;
		if (extractedBitNum == 32) {
			hiddenByteNum = bitset<32>(hiddenByteNumBits).to_ulong();
			cout << "Trong hinh co " << hiddenByteNum << " byte du lieu duoc giau" << endl;
		}
	}
	return true;
}

int getExtractResultBytes(byte*& resultBytes, string resultBits, int hiddenByteNum) {
	resultBytes = new byte[hiddenByteNum];
	for (int k = 0; k < hiddenByteNum; k++)
		resultBytes[k] = _8BitToDec(resultBits.substr(k * 8, 8));
	return hiddenByteNum;
}

int extract(BMPFile fromBMP, byte*& resultBytes) {
	string hiddenByteNumBits = "";	//biến này để lấy từ từ từng bit đầu cho tới khi đủ 32 bit thì chuyển sang biến hiddenByteNum
	int hiddenByteNum = -1;
	string resultBits = "";
	int extractedBitNum = 0;

	for (int i = fromBMP.dib.height - 1; i >= 0; i--) {
		for (int j = 0; j < fromBMP.dib.width; j++) {
			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].blue, resultBits, extractedBitNum, hiddenByteNum, hiddenByteNumBits))
				return getExtractResultBytes(resultBytes, resultBits, hiddenByteNum);

			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].green, resultBits, extractedBitNum, hiddenByteNum, hiddenByteNumBits))
				return getExtractResultBytes(resultBytes, resultBits, hiddenByteNum);

			if (!extractFromColorChannelByte(fromBMP.pixelArray.pixels[i][j].red, resultBits, extractedBitNum, hiddenByteNum, hiddenByteNumBits))
				return getExtractResultBytes(resultBytes, resultBits, hiddenByteNum);
		}
	}

	return 0;
}