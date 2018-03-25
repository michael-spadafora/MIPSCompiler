#ifndef HW_H
#define HW_H

#include "const.h"
#include "instruction.h"
int hatoi(char * string);
int instructionFromInput(Instruction* ip, char* input, int len);
int strEquals(char* str1, char*str2);
int flipEndian(int num);
#endif
