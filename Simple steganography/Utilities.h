#pragma once
#include <iostream>
#include <cmath>
#include <bitset>
#include "BMP.h"
using namespace std;
#define BLOCK_SIZE 1024
typedef unsigned char byte;

string decTo8Bit(int dec);

int _8BitToDec(string bin);

char getBitAtPos(int value, int pos);

byte modifyBit(int value, int pos, int toBit);
