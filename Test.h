#ifndef _CRUMTEST_
/* Test is a struct containing the information related to a single unit test in the `crumtest` program. It simply has two strings, the input and expected output.
 * The strings will be dynamically allocated, so they need to be deleted when the struct is done. `destroyTest` and `ndestroyTest` are provided for this purpose. */
#include <stdlib.h>
#include <stdio.h>

struct Test {
    char *input; // input to be fed into the tested program
    char *output; // expected output of the program
};
/* `gentests` is a function to read through file named by the argument and produce an array of `Test`s representing the test cases defined in the file.
 * Test definition syntax is defined in the README */
int genTests(const char *file, struct Test **tests, int *count);
/* `destroyTest` deallocates a `Test`, pointed to by the argument */
void destroyTest(struct Test *victim);
/* `ndestroyTest` destroys n `Test`s, in an array pointed to by the first arg */
void ndestroyTest(struct Test *victims, int n);

#define _CRUMTEST_
#endif
