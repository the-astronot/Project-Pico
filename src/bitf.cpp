#include "bitf.h"

void bitf::set(char &byte, int index) {
  byte |= 1 << index;
}

void bitf::unset(char &byte, int index) {
  byte &= ~(1 << index);
}

void bitf::toggle(char &byte, int index) {
  byte ^= 1 << index;
}

int bitf::get(char &byte, int index) {
  return ((byte >> index) & 1);
}

void bitf::toHex(char byte, char (&hex)[2]) {
  int sum;
  for(int i=0; i<2; i++) {
    sum=0;
    for(int j=0; j<4; j++) {
      if(get(byte,(4*i)+j) == 1) {
        sum += pow(2,j);
      }
    }
    hex[1-i] = hexarray[sum];
  }
}

void bitf::toBinary(char byte, char (&binary)[8]) {
	for (int i=0; i<8; i++) {
    binary[7-i] = get(byte,i) + '0';
  }
}

std::string bitf::toBinaryString(char byte) {
	std::string bits;
	int bit;
	for (int i=0; i<8; i++) {
    bit = get(byte,7-i);
		if (bit == 0) {
			bits += "0";
		} else {
			bits += "1";
		}
  }
	return bits;
}
