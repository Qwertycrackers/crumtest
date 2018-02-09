/* This program is intended to aid in the testing of CS 159 homework and lab problems.
 * These problems generally have extremely precise examples, and the final output of the program needs to exactly match the example case.
 * Thus, `crumtest` will accept an executable file and a test file as arguments, running the executable while directing its stdin and stdout
 * to check the output and give the input.
 *
 * Author: Joseph Gerardot   */
#include <stdio.h>
#include "Test.h"

int main(int argc, char **argv) { // we're going to accept command-line args
    TestCase *cases; // a dynamic array of the test cases
    int numCases; // the number of cases
    int i = 0; // interator over the test cases
    char *testFile; // the name of the file containing tests. Given from argv
    char *testExec; // the name of the executable to test. Given from argv
    if(argc < 3) {
        printUsage();
        return 0;
    } // tell the user what to do, if they gave wrong number of arguments.
    testExec = argv[1]; // the second element (first argument) should be the executable.
    testFile = argv[2]; // the third element (second argument) should be the test file.

    if(genTestCases(testFile, &cases, &numCases) != 0) {
        puts("Error encountered when generating test cases! Aborting...");
        return -1;
    } // generate the test cases, and complain if something goes wrong. `cases` should now be an array of TestCases.
    while(i < numCases) {
        switch(test(testExec,cases + i++)) { // switch on result of test, increment i along with pointer arithmetic
            case 0:
                printf("Test %d resolved successfully.\n", i+1);
                break; 
