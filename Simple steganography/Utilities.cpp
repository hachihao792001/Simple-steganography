#include "Utilities.h"

string decTo8Bit(int dec)
{
	/*string bin = "";
	int i = 1;
	while (dec > 0) {

		bin = char((dec % 2) + '0') + bin;
		dec = dec / 2;
		i *= 10;
	}

	while (bin.length() < 8)
		bin = '0' + bin;

	return bin;*/

	return bitset<8>(dec).to_string();
}

int _8BitToDec(string bin)
{
	/*int dec = 0;
	int i = 0;
	for (int i = bin.length() - 1; i >= 0; i--) {
		if (bin[i] == '1')
			dec += pow(2, bin.length() - i - 1);
	}
	return dec;*/

	return bitset<8>(bin).to_ulong();
}

char getBitAtPos(int value, int pos) {
	//copied from stackoverflow
	return ((value >> pos) & 1) + '0';
}

byte modifyBit(int value, int pos, int toBit)
{
	//copied from geeksforgeeks 
	int mask = 1 << pos;
	return (value & ~mask) | ((toBit << pos) & mask);
}