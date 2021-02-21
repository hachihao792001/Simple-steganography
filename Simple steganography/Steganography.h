#pragma once
#include "Utilities.h"

void hideToColorChannelByte(byte& colorChannel, string fromBits, int bitNum, int& currentHidePos);

void hide(byte* fromBytes, int byteNum, BMPFile& toBMP);

bool extractFromColorChannelByte(byte colorChannel, string& current8Bits, string& toByteStr, int& currentBitPos, int& hiddenByteNum, string& hiddenByteNumStr);

int extract(BMPFile fromBMP, byte*& toByte);
