#include "Test.h"

int countTests(FILE *file); // internal function to count tests
int genTest(FILE *file, struct Test *test); // generate a test from a section of a file

int genTests(const char *fileName, struct Test **tests, int *count) {
  FILE *file; // file descriptor of the test file to parse
  char c; // current char off the stream
  int error = 0; // error tracker
  int i = 0; // iterator through the test array

  file = fopen(fileName, "r"); // open the file read-only
  *count = countTests(file); // find out how many test cases are in the file
  *tests = malloc(sizeof(struct Test) * *count); // alloc enough space for `count` tests
  rewind(file); // return to the beginning of the file

  do {
    c = getc(file); // put the next char into c
    if(c == '[') { // if beginning of test is found
       error = genTest(file, *tests + i++); // create a test from the section of the file
    }
  } while(c != EOF && error == 0); // go to the end of the file or when a problem is encountered
  return error;
}

int countTests(FILE *file) {
  char c; // current character off the stream
  int count = 0; // the count of tests
  do {
    c = getc(file); // get the next char
    if(c == '[') {
      #ifdef CRUM_DEBUG
        puts("Found a bracket");
      #endif
      count++;
    } // count every opening brace as a test. Clearly will cause problems if the input is supposed to include opening braces, but that's for another time.
  } while(c != EOF);
  return count; // count is what we were searching for
}

int genTest(FILE *file, struct Test *test) { // generate a single test case, by reading a section of a file
  char c; // the current char off the stream
  char *buf; // buffer for the memory stream
  size_t ssize; // size of the buffer
  FILE *stream; // fd of the memory stream for building the cases

  stream = open_memstream(&buf, &ssize); // open the stream 
  while(1) {
    c = getc(file); // get the next char off the file stream
    if(c == ']')
      break;
    if(c == EOF) {
      return -1; // shouldn't reach the end in here
    }
    putc(c,stream); // if it's not the end of the input section or end of the file, put the character into our buffer
  }
  fclose(stream); // close our buffer and flush the input
  test->input = buf; // make `input` field of the test point to our buffer, which should now contain the input for the test case (between [])
  
  do {
    c = getc(file);
    if(c == EOF) {
      return -1; // shouldn't reach the end in here
    }  
  } while(c != '{'); // iterate through stream until then opening '{' of the output section is found

  stream = open_memstream(&buf, &ssize); // new stream for the next section
  while(1) {
    c = getc(file);
    if(c == EOF) {
      return -1;
    }
    if(c == '}') {
      break;
    }
    putc(c,stream); // if c is neither of the special values, put it in the buffer
  }
  fclose(stream); // done with adding stuff, so close the stream 
  test->output = buf; // output is the string in the buffer
  return 0; // if we get here, everything was successful
}

void destroyTest(struct Test *victim) { // destroy the heap memory of a test so it may safely be deallocated
  free(victim->input); // free the input string
  free(victim->output); // free the output string
}

void ndestroyTest(struct Test *victims, int n) { // destroy n victims
  int i = 0;
  while(i < n) {
    destroyTest(victims + i++);
  }
}
