#pragma once
#include "Utilities.h"

void hide(byte* fromBytes, int byteNum, BMPFile& toBMP);

bool extractFromColorChannelByte(byte colorChannel, string& resultBits, int& extractedBitNum, int& hiddenByteNum, string& hiddenByteNumBits);

int getExtractResultBytes(byte*& resultBytes, string resultBits, int hiddenByteNum);

int extract(BMPFile fromBMP, byte*& resultBytes);
