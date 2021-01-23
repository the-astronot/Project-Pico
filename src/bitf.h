#ifndef __bitf_h__
#define __bitf_h__

#include <string>
#include <math.h>

namespace bitf {

  // Hex
  static char hexarray[] = {'0','1','2','3','4','5','6','7',
                            '8','9','a','b','c','d','e','f'};

  // Simplified Bit Manipulation
  void set(char &byte, int index);
  void unset(char &byte, int index);
  void toggle(char &byte, int index);
  int get(char &byte, int index);
  void toHex(char byte, char (&hex)[2]);
	void toBinary(char byte, char (&binary)[8]);
	std::string toBinaryString(char byte);
}

#endif
