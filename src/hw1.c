#include "hw1.h"
#include <stdbool.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 */

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */




int validargs(int argc, char **argv) //this is an array of char arrays aka an array of strings
{
	if (argc<=1 || argc>7){
		return 0;
	}
	int hasHadFlag = 0;
	int hFlag = 0;
	int aFlag = 0;
	int dFlag = 0;
	int bFlag = 0;
	char* address;
	int addressNum;
	int eFlag = 0;
	int bigEnd = 0;
	int littleEnd = 0;
	int howMany= 0;
    for (int i = 1; i < argc; i++){

        if (argv[i][0] == '-'){ //check if something is a flag or not.
			//-\0
        	char next = argv[i][1]; // this will give a seg fault if the argument is a dash with nothing else
        	if (next == 0){
        		return 0;
        	}
        	if (argv[i][2]!=0){
        		return 0;
        	}
			if( next == 0){
				return 0;
			}
        	if (next == 'h'){ //check if current flag is h flag
        		if (i!=1){
        			return 0;
        		}
        		hFlag = 1;
        		break; //ignore all the other stuff if h is used
        	}

        	else if (next == 'a'){ //check if the current flag is an a flag
        		if (aFlag || bFlag || dFlag || eFlag){
        			return 0;
        		}
        		aFlag = 1;
        	}

        	else if (next == 'd') {
        		if (aFlag || bFlag || dFlag || eFlag){
        			return 0;
        		}
        		dFlag = 1;
        	}
//check base address flag
        	else if (next == 'b'){
        		if (bFlag || eFlag){
        			return 0;
        		}

        		bFlag = 1;

        		i++;

        		if (i>=argc){ //if we went too far, there is something wrong.
        			return 0;
        		}

        		address = argv[i];

        		int j = 0;
        		//here we check if the address is valid
        		char currChar = address[j];
        		while (currChar != 0){ //checks if all the numbers in the address are hex values


        			if (!(currChar>='a' && currChar<='f') && !(currChar>='A' && currChar <='F') && !(currChar>='0' && currChar<='9')){
        				return 0;
        			}
        			j++;
        			currChar = address[j];
        			if (j > 8){
        				return 0; //for some reason, this keeps breaking
        			}

        		}
        		addressNum = hatoi(address);

        		if (addressNum % 4096 != 0){
        			return 0;
        		}

        	}

        	else if (next == 'e'){ //check if other flags have been had
        		if(eFlag){
        			return 0;
        		}
        		eFlag = 1;

        		i++;
        		char* endian = argv[i];

        		bigEnd = (endian[0] == 'b');
        		littleEnd = (endian[0] == 'l');
        		if (endian[1] != '\0'){ //check if it is too long
        			return 0;
        		}

        		if (!bigEnd && !littleEnd){
        			return 0;
        		}
        	}
        	else return 0;


       	} else {
       		return 0;
       	}



    }

    int flagSum = 0;

	if (dFlag){
		flagSum = 2;
	}

	if (bigEnd){
		flagSum += 4;
	}

	if (bFlag){
		flagSum+= addressNum;


	}

	if (hFlag!=0){
		flagSum=1;
	}

	global_options = flagSum;


    return 1;
}
/*
int strlen(char * string){
	int len = 0;
	char currChar = ' ';
	while (currChar != 0){
		len++;
	}
	return len;
}
*/
int hatoi(char * string){
	char currChar  = string[0];
	int length = 0;
	while (currChar!='\0'){
		length++; //count the length of the current string
		currChar = string[length];
	}

	int multiplier = 1;
	int cumsum = 0;

	for (int i = length-1; i >=0; i--){
		currChar = string[i];
		int value;
		if ('0' <= currChar && currChar <= '9') value = currChar - '0';
		else if ('a'<= currChar && currChar <= 'f') value = 10 + currChar - 'a';
		else if ('A'<= currChar && currChar <= 'F') value = 10 + currChar - 'A';

		int currentValue = multiplier * value;
		cumsum += currentValue;

		multiplier = multiplier*16;
	}
	return cumsum;
}

int flipEndian(int num){

	int  b0,b1,b2,b3;
	int reversed;

	b0 = (num & 0x000000ff) << 24u;
	b1 = (num & 0x0000ff00) << 8u;
	b2 = (num & 0x00ff0000) >> 8u;
	b3 = (num & 0xff000000) >> 24u;

	reversed = b0 |b1 |b2 |b3;
	return reversed;

}

int instructionFromInput(Instruction* ip, char input[], int len){
	int i = -1;
	int loc = -1;
	int args[3];
	for (i = 0;i<64; i ++){
		//char input[20] = "add $3,$3,$3";

		Instr_info info = instrTable[i];
		char * format = info.format;

		///char format[20] = "add $%d,$%d,$%d";
		//printf("format:%s - input:%s\n", info.format, input);

		int argCount = 0;

		for (int j = 0; j<3; j++){
			if (info.srcs[j]!=NSRC){
				argCount++;
				//printf("argNum:%d\n", argCount);
			}
		}

		int arg1 = 0, arg2 = 0, arg3 = 0;
		int scanRet = sscanf(input, format, &arg1, &arg2, &arg3);

		args[0] = arg1;
		args[1] = arg2;
		args[2] = arg3;

		//printf("scan:%d\n", scanRet);
		if (scanRet >0){

			if (scanRet != argCount){
				return 0;
			}
			loc = i;
			break;
		}
	}


	if (strEquals(input, "syscall")){


		Opcode opcode;
		if (strEquals(input, "syscall"))
			opcode = OP_SYSCALL;
		//else opcode = OP_RFE;

		for (int i = 0; i < 64; i++){
			if (instrTable[i].opcode == opcode){
				loc = i;
				//printf("%d", loc);
				break;
			}
		}
	}

	if (loc == -1){
			//printf("thing not found\n");
			return 0;
	}

	Instr_info info = instrTable[loc];

	(*ip).info = &info;
	for (int i = 0 ; i < 3; i++){
		(*ip).args[i] = args[i];
		if (info.srcs[i] == RS){
			(*ip).regs[0] = args[i];
		} else if (info.srcs[i] == RT){
			(*ip).regs[1] = args[i];
		} else if (info.srcs[i] == RD){
			(*ip).regs[2] = args[i];
		} else if (info.srcs[i] == EXTRA){
			(*ip).extra = args[i];
		}
	}


	return 1;



}

int strEquals(char* str1, char*str2){
//printf("%s,%s", str1, str2);
	int len1=0, len2=0;
	while (str1[len1] != 0){
		len1++;
	}

	while (str2[len2] != 0){
		len2++;
	}
	len1--;
	if (len1!=len2){
		//printf("%d,%d", len1, len2);
		return 0;
	}

	for (int i = 0; i < len1; i++){
		if (str1[i] != str2[i] && str2[i] != '\n' && str1[i] != '\n'){
			return 0;
		}
	}
	return 1;

}

/**
 * @brief Computes the binary code for a MIPS machine instruction.
 * @details This function takes a pointer to an Instruction structure
 * that contains information defining a MIPS machine instruction and
 * computes the binary code for that instruction.  The code is returne
 * in the "value" field of the Instruction structure.
 *
 * @param ip The Instruction structure containing information about the
 * instruction, except for the "value" field.
 * @param addr Address at which the instruction is to appear in memory.
 * The address is used to compute the PC-relative offsets used in branch
 * instructions.
 * @return 1 if the instruction was successfully encoded, 0 otherwise.
 * @modifies the "value" field of the Instruction structure to contain the
 * binary code for the instruction.
 */
int encode(Instruction *ip, unsigned int addr) {

// we need to do sscanf(instruction.info., instrtable[i].info, arg1, arg2, arg3)
	Instr_info *infop = (*ip).info;
	Instr_info info = *infop;
	Opcode opcode = info.opcode;
	//Opcode current = opcodeTable[0];

	int value = 0x00000000;

	int loc = -1;
	for (int i = 0; i <64; i++){
		if (opcode == opcodeTable[i]){
			loc = i;
			//printf("ffff");
			break;
		}
	}


	if (opcode == OP_RFE){
		return 0;
	}
	int specLoc = -1;
	int bcondCode = -1;


	if (loc == -1){ //if not found in regular opcodes then we need to look through specialtable
		//printf("loc not found");
		loc = 0;
		specLoc = -1;
		for (int i = 0; i < 64; i++){
			if (opcode == specialTable[i]){
				specLoc = i;
				break;
			}
		}
		if (opcode == OP_SYSCALL){
		value = value | specLoc;
		(*ip).value = value;

		//printf("asdfasdf");
		return 1;
	}

		if (specLoc == -1){ //if not in specTable, then we need to see if it is a valid bcond
			loc = 1;
			bcondCode = -1;
			if (opcode == OP_BLTZ){
				bcondCode = 0b00000;
			} else if (opcode == OP_BGEZ){
				bcondCode = 0b00001;
			} else if (opcode == OP_BLTZAL){
				bcondCode = 0b10000;
			} else if (opcode == OP_BGEZAL){
				bcondCode = 0b10001;
			}
		}
	} //everything above this line should be okay

	value = value | (loc << 26);
	if (specLoc != -1){
		value = value | specLoc;
	} else if (bcondCode != -1){
		value = value | (bcondCode<<16);
	}

	for (int i = 0; i < 3; i++){
		Source src = info.srcs[i];
		if (info.opcode == OP_SYSCALL || info.opcode == OP_RFE || info.opcode == OP_UNIMP){
			//printf("woopdy");
			break;
		}

		if (src == RS){
			char reg = (*ip).regs[0];
			int regNum = reg;
			value = value | (regNum<<21);
		} else if (src == RT){
			char reg = (*ip).regs[1];
			int regNum = reg;
			value = value | (regNum<<16);
		} else if (src == RD){
			char reg = (*ip).regs[2];
			int regNum = reg;
			value = value | (regNum<<11);
		} else if (src == EXTRA){
			if (info.opcode == OP_BREAK){
				int extra = (*ip).extra;
				extra = extra & 0x000fffff;
				extra = extra << 6;
				value = value | extra;
			} else if (info.type == RTYP){
				int extra = (*ip).extra;
				extra = extra&0x0000001f;
				extra = extra << 6;
				value = value | extra;
				//do stuff here
			} else if (info.type == ITYP){
				if (opcode == OP_BEQ || opcode== OP_BGEZ ||  opcode == OP_BGEZ ||  opcode == OP_BGEZAL || opcode  ==OP_BGTZ || opcode ==  						OP_BLEZ || opcode ==  OP_BLTZ || opcode ==  OP_BLTZAL || opcode == OP_BNE) {
			 int extra = (*ip).extra;
			 extra = extra & 0x0000ffff;

			 extra = extra - 4 - addr;
			 extra  = extra >> 2;
			 value = value | extra;
			}
			else {
				int extra = (*ip).extra;
				extra = extra & 0x0000ffff;
				value = value | extra;
			}

			} else if (info.type == JTYP){
				int extra = (*ip).extra;
				int baseAddrToCheck = global_options & 0xF0000000;

				extra = (extra - (addr&0xfc000000)) >> 2;
				if ((baseAddrToCheck & extra) != baseAddrToCheck){
					return 0;
				}

				//extra = (extra -4) >> 2;
				value = value|extra;
			}
		} else if (src == NSRC) {
			continue;
		}
	}


	(*ip).value = value;
    return 1;
}

/**
 * @brief Decodes the binary code for a MIPS machine instruction.
 * @details This function takes a pointer to an Instruction structure
 * whose "value" field has been initialized to the binary code for
 * MIPS machine instruction and it decodes the instruction to obtain
 * details about the type of instruction and its arguments.
 * The decoded information is returned by setting the other fields
 * of the Instruction structure.
 *
 * @param ip The Instruction structure containing the binary code for
 * a MIPS instruction in its "value" field.
 * @param addr Address at which the instruction appears in memory.
 * The address is used to compute absolute branch addresses from the
 * the PC-relative offsets that occur in the instruction.
 * @return 1 if the instruction was successfully decoded, 0 otherwise.
 * @modifies the fields other than the "value" field to contain the
 * decoded information about the instruction.
 */
int decode(Instruction *ip, unsigned int addr) {

	Instruction instruction = *ip;
	unsigned int val = instruction.value;
	unsigned int codeVal = val >> 26;

	//printf("\n%d\n\n", codeVal);


	Opcode opcode = opcodeTable[codeVal]; //gets the original opcode


	if (opcode == SPECIAL){ //processes the opcode for SPECIAL
		int specVal = val & 0x0000003f;
		codeVal = specVal;
		opcode = specialTable[codeVal];
		//printf("zzzzz");
	}

	else if (opcode == BCOND){ //processes opcode for BCOND
		int bcond = (val & 0x001f0000) >> 16;
		opcode = ILLEGL;
		if (bcond == 0b00000){
			opcode = OP_BLTZ;
		} else if (bcond == 0b00001){
			opcode = OP_BGEZ;
		} else if (bcond == 0b10001){
			opcode = OP_BLTZAL;
		} else if (bcond == 0b10000){
			opcode = OP_BGEZAL;
		}
	}
//printf("not found in table");
	//opcode = opcodeTable[codeVal]; //this is the "name" of the instruction

	if (opcode == ILLEGL || opcode == OP_UNIMP || opcode == OP_RFE){
		//printf("not found in tablze");
		return 0;
	}
	int locInTable = -1;
	for (int i = 0 ; i < 64; i++){
		if (instrTable[i].opcode == opcode){
			locInTable = i;
			break;
		}
	}
 	if (locInTable == -1){
 		//printf("not found in table");
 		return 0;
 	}

	//////////////////////////////////////////
	Instr_info info = instrTable[locInTable]; // gets the info. youre going to find the
	//printf("\n\n codeval:%d \n\n" ,codeVal);
	Type typez = info.type;
	//Type types = NTYP;

	//typedef enum type { NTYP, ITYP, JTYP, RTYP } Type;

	// info field is set
	Instr_info * infop = &info;
	(*ip).info = infop;

	int rs = (val & 0x03E00000 )>>21; // we need to move it over and convert it to a char
	int rt = (val & 0x001f0000) >> 16;
	int rd = (val & 0x0000f800) >> 11;

	char rschar = rs;
	char rtchar = rt;
	char rdchar = rd;


	//printf("register:%c\n", rdchar);


	(*ip).regs[0] = rschar;
	(*ip).regs[1] = rtchar;
	(*ip).regs[2] = rdchar; //register fields are set





	for (int i = 0; i < 3; i++){ //args field is set
		Source s = info.srcs[i];
		//printf("asdfljaslkdfs");
		if (s == RS){
			(*ip).args[i] = rs;
			//printf("asdfljaslkdfs");
		} else if (s == RT){
			(*ip).args[i] = rt;
			//printf("asdfljaslkdfs");
		} else if (s == RD){
			(*ip).args[i] = rd;
			//printf("asdfljaslkdfs");
		} else if (s == NSRC) {
			(*ip).args[i] = 0;
			continue;
		} else if (s == EXTRA) { //extra field is set
			if (opcode == OP_BREAK){
				//Instruction instruction = *ip;
				(*ip).args[i] = val & 0x03ffffc0;
				(*ip).extra = (*ip).args[i];
			} else if (typez == RTYP) {
				(*ip).args[i] = (val & 0x000007c0) >> 6;

				(*ip).extra = (*ip).args[i];
			} else if (typez == ITYP) {
				if ((val & 0x00008000)){

					(*ip).args[i] = (val & 0x0000ffff) | 0xffff0000;
				} else {
					(*ip).args[i] = val & 0x0000ffff;
				}


				if (opcode == OP_BEQ || opcode== OP_BGEZ ||  opcode == OP_BGEZ ||  opcode  == OP_BGEZAL || opcode  ==OP_BGTZ ||
					opcode ==  OP_BLEZ || opcode ==  OP_BLTZ || opcode ==  OP_BLTZAL || opcode == OP_BNE)  {

					(*ip).args[i] = ((*ip).args[i] << 2) + addr + 4;
					(*ip).extra = (*ip).args[i];
				}
			} else if (typez == JTYP){
				(*ip).args[i] = ((val & 0x03ffffff) << 2 )  + (addr&0xfc000000);
				(*ip).extra = (*ip).args[i];
			}

		}
	}
    return 1;
}
