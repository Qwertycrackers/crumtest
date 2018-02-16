/* This program is intended to aid in the testing of CS 159 homework and lab problems.
 * These problems generally have extremely precise examples, and the final output of the program needs to exactly match the example case.
 * Thus, `crumtest` will accept an executable file and a test file as arguments, running the executable while directing its stdin and stdout
 * to check the output and give the input.
 *
 * Author: Joseph Gerardot   */
#include <stdio.h>
#include <stdlib.h>
#include "Test.h"
#include "ExecTest.h"

void printUsage(); // function to print the proper way to use this program

int main(int argc, char **argv) { // we're going to accept command-line args
  struct Test *cases; // a dynamic array of the test cases
  int numCases; // the number of cases
  int i = 0; // interator over the test cases
  int status = 0; // tracker for success
  char *testFile; // the name of the file containing tests. Given from argv
  char *testExec; // the name of the executable to test. Given from argv
  char *error = NULL; // string to hold the dianostic errors
  if(argc < 3) {
    printUsage();
    return 0;
  } // tell the user what to do, if they gave wrong number of arguments.
  testExec = argv[1]; // the second element (first argument) should be the executable.
  testFile = argv[2]; // the third element (second argument) should be the test file.

  if(genTests(testFile, &cases, &numCases) != 0) {
    puts("Error encountered when generating test cases! Aborting...");
    return -1;
  } // generate the test cases, and complain if something goes wrong. `cases` should now be an array of TestCases.
  #ifdef CRUM_DEBUG
  printf("%d test cases registered. Entering testing loop\n",numCases);
  #endif
  while(i < numCases) {
    #ifdef CRUM_DEBUG
    printf("Beginning test %d\n", i + 1);
    #endif
    switch(execTest(testExec,cases + i++, &error)) { // switch on result of test, increment i along with pointer arithmetic
      case 0:
        printf("Test %d resolved successfully.\n", i+1);
        break; 
      case -1:
        printf("Test %d failed. Reason: \n%s", i + 1, error);
        free(error);
        error = NULL;
        break;
      case -2:
        printf("crumtest itself encountered a problem. Error: \n%s", error);
        status = -1;
        free(error);
        error = NULL;
        break;
      default:
        puts("Default case reached.");
    }
  }
  printf("%d tests completed!\n", numCases); // tell how many tests are completed
  return status;
}

void printUsage() {
  puts("crumtest is a unit-tester for simple CS homework executables.\nv0.0.1\nAuthor: Joseph Gerardot\nUsage:\ncrumtest [EXECUTABLE] [TEST FILE]");
}
