#pragma once
#include "Utilities.h"

int GetFileBytes(string fileName, byte*& result);

void CreateFileFromBytes(string fileName, byte* bytes, int byteCount);
