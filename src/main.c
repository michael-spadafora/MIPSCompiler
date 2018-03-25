#include <stdlib.h>
#include <stdio.h>
#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

#include "hw1.h"
#include "debug.h"

int main(int argc, char **argv)
{
//use freads for binary data
//fgets method to read from stdin fgets(string, 120, stdin) for encoding
//fread for decoding
//fwrite to write binary to a file (for encode)
//
	//printf("\nworks til here\n");
    if(!validargs(argc, argv))

        USAGE(*argv, EXIT_FAILURE);
    debug("Options: 0x%X", global_options);
    if(global_options & 0x1) {
        USAGE(*argv, EXIT_SUCCESS);
    }

    int currAddress = global_options & 0xFFFFF000; //gets the base address. if -b was spec, then it should get the num after b, and if not,
    //this will get the number 0

    if (global_options & 2){
		//printf("\ndecode\n");

		char  words[120][120];

		int val = -1;
		int i = 0;

		while (!feof(stdin)){

	//printf("is dis the seg fault\n");
			fread(&val, sizeof(int), 1, stdin);
			//printf("is dis the seg fault\n");

			Instruction instr;
			if (global_options & 4){
				val = flipEndian(val);
			}
			instr->value = val;

			if (!decode(instr, currAddress)){
				//printf("failure\n");
				return EXIT_FAILURE;
			}
			//printf("is dis\n");
			if (val!= -1 && !feof(stdin)){

				printf(instr->info->format, instr->args[0], instr-> args[1], instr->args[2]);
				printf("\n");

			}
			currAddress+=4;
		}
		//printf("%d\n", str);

		//scanf("%[^\n]",str); //this reads in a full line

		//Instrution ins = instructionFromInput(input);

    } else {
    	//printf("\nencode\n");
    	int values[1000];

    	int i = 0;
    //keep doing this while not eof
		while (!feof(stdin)){
			char str[120];

    		fgets(str, 120, stdin);
    		if (feof(stdin)){
   	 			break;
   	 		}
    		//scanf("%[^\n]",str);
    		int len = 0;
			//printf("%saa\n",str);
    		for (len = 0; len < 100; len++){
    			if (str[len] == 0){
    				break;
    			}
    		}

			Instruction  instr;
			Instruction * ins = instr;
   		 	int ret = instructionFromInput(ins, str, len);
   		 	if (!ret){
   		 		//printf("\nfailurez\n");
   		 		return EXIT_FAILURE;
   		 	}
    		//printf("%d",ins->args[1]);
   	 		if (encode(ins, currAddress) == 0){
   	 		//printf("\nfailure\n");
   	 			return EXIT_FAILURE;
   	 		}

			//printf("gets to here");
   	 		//values[i] = ins->value;
			if (!feof(stdin)){
				if (global_options & 4){
					ins ->value = flipEndian(ins->value);
				}
   	 			fwrite(&(ins->value), sizeof(int),1,stdout); //for some reason it goes through one extra time
   	 		}


    		currAddress+=4;
    	}

		if (i == 0){
			return EXIT_SUCCESS;
		}



    }


    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
