#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

/*Test(hw1_tests_suite, validargs_help_test) {
    int argc = 2;
    char *argv[] = {"bin/hw1", "-h", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 1;
    unsigned int opt = global_options;
    unsigned int flag = 0x1;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid args.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt & flag, flag, "Least significant bit not set for -h. Got: %d",
		 opt);
}

Test(hw1_tests_suite, wrongFlagWithBaseValue) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-d", "fffffF000", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt & flag, flag, "Disassemble bit wasn't set. Got: %d",
		 opt);
}

Test(hw1_tests_suite, baseValueOnly) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-b", "fffffF000", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt & flag, flag, "Disassemble bit wasn't set. Got: %d",
		 opt);
}

Test(hw1_tests_suite, checkBaseAddress) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-a", "-b", "fffff000", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 1;
    unsigned int opt = global_options;
    unsigned int flag = 0xfffff000;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}

Test(hw1_tests_suite, repeatFlags) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-a", "-a", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}


Test(hw1_tests_suite, repeatFlags2) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-d", "-d", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}


Test(hw1_tests_suite, repeatFlags4) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-e", "-e", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}


Test(hw1_tests_suite, wrongOrder1) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-b", "-d", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}





Test(hw1_tests_suite, endian) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-e", "b", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    unsigned int opt = global_options;
    unsigned int flag = 0x0;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt, flag, "flag not correct. got: %d",
		 opt);
}



Test(hw1_tests_suite, helpMenuWithOtherArgs) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-h", "-b", "fffffF000", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 1;
    unsigned int opt = global_options;
    unsigned int flag = 0x1;
    cr_assert_eq(ret, exp_ret, "Invalid return for valid argsz.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt & flag, flag, "Disassemble bit wasn't set. Got: %d",
		 opt);
}






Test(hw1_tests_suite, help_system_test) {
    char *cmd = "bin/hw1 -h";

    // system is a syscall defined in stdlib.h
    // it takes a shell command as a string and runs it
    // we use WEXITSTATUS to get the return code from the run
    // use 'man 3 system' to find out more
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS",
		 return_code);
}*/

Test(hw1_tests_suite, decodetest1){
	char *cmd = "bin/hw1 -d";
	int binInstruct = 0x00c72820;

	Instruction instruct[1000];
	instruct -> value = binInstruct;

	//Instruction* point = &instruct;

	//Instr_info info = *(instruct.info);
	int ret = decode(instruct, 0);
	printf("%x\n", instruct -> value);
	printf("%c\n", instruct -> regs[0]);
	printf("%c\n", instruct -> regs[1]);
	printf("%c\n\n", instruct -> regs[2]);

	printf("%d\n", instruct -> args[0]);
	printf("%d\n", instruct -> args[1]);
	printf("%d\n", instruct -> args[2]);

	char expreg1 = 7;
	char expreg2 = 6;
	char expreg3 = 5;


	//printf("%s\n", (*instruct.info).format);
	//printf("%d\n", instruct.value);
	//printf("%d\n", info.opcode);
	//for (int i  = 0 ; i < 3; i ++){
	//	printf("%d\n", instruct.regs[i]);

	//}*/
}

