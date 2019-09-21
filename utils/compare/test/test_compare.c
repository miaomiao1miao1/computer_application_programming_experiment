#include "../../boolean/boolean.h"
#include "../../print/print.h"
#include "../compare.h"
#include <stdio.h>

typedef struct testcase_char {
    char    arg1;
    char    arg2;
    boolean want_res;

} testcase_char;

typedef struct testcase_string {
    char *  arg1;
    char *  arg2;
    boolean want_res;

} testcase_string;

int main() {
    boolean       pass = T;
    testcase_char test_char[] = {
        {arg1: 'a', arg2: 'b', want_res: F},
        {arg1: 'a', arg2: 'a', want_res: T},
        {arg1: '0', arg2: '0', want_res: T},
        {arg1: '1', arg2: '!', want_res: F},
    };
    testcase_string test_string[] = {
        {arg1: "Hello", arg2: "Hello", want_res: T},
        {arg1: "Hi", arg2: "Hi0", want_res: F},
        {arg1: "Hi", arg2: "Hi\0", want_res: T},
        {arg1: "ABC\0B", arg2: "ABC\0A", want_res: T},
    };

    for (int i = 0; i < sizeof(test_char) / sizeof(testcase_char); ++i) {
        testcase_char *tc = &test_char[i];
        if (compare_char(tc->arg1, tc->arg2) != tc->want_res) {
            pass = F;
            print_err("Check error at compare_char('%c','%c')=%d\n", tc->arg1,
                      tc->arg2, tc->want_res);
        }
    }

    for (int i = 0; i < sizeof(test_string) / sizeof(testcase_string); ++i) {
        testcase_string *tc = &test_string[i];
        if (compare_string(tc->arg1, tc->arg2) != tc->want_res) {
            pass = F;
            print_err("Check error at compare_string('%s','%s')=%d\n", tc->arg1,
                      tc->arg2, tc->want_res);
        }
    }

    if (pass) {
        print_ok("All testcase passed\n");
        return 0;
    } else {
        print_err("Some error\n");
        return 1;
    }
}