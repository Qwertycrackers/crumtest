#include "ExecTest.h"

int diagnose(int result, const char *expected, char **diag);
int swrite(int fd, const char *s); // function to write a string into a memory buffer

int execTest(const char *executable, struct Test *testCase, char **error) {
  int in[2]; // pipe for input
  int out[2]; // pipe for output
  int pid; // process id returned from the fork
  int wstatus; // receiver for the status of child program
  #ifdef CRUM_DEBUG
  puts("Beginning execution of a test.");
  #endif
  pipe(in); // create the pipe mocking `stdin`
  pipe(out); // create the pipe mocking `stdout`
  #ifdef CRUM_DEBUG
  puts("Created pipes. Beginning fork.");
  #endif
  pid = fork(); // we forked babeeee
  
  if(pid == -1) { // something went wrong
    *error = "Failed to successfully fork process.\n"; // make `error` meaningful
    return -2; // error code
  }
  if(pid > 0) { // fork successful, parent process
    close(in[0]);
    close(out[1]); // don't need to write to `out` or read from `in`
    #ifdef CRUM_DEBUG
    puts("Writing input into input buffer.");
    #endif
    swrite(in[1], testCase->input); // write the input into the input buffer
    do {
      #ifdef CRUM_DEBUG
      puts("Beginning to wait.");
      #endif
      wait(&wstatus); // wait for a change in child process
      #ifdef CRUM_DEBUG
      printf("Wait returned. wstatus: %d\n",wstatus);
      #endif
    } while(!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    if(WIFEXITED(wstatus))
      if(WEXITSTATUS(wstatus) < 0) { // if the child process finished with anything other than 0
        *error = "Child process exited with status less than zero.\n";
        return -1;
      }
    if(WIFSIGNALED(wstatus)) {
      *error = "Child process was terminated by a signal.";
      return -1;
    }
    close(in[1]); // close the write side of `in`, as we are done with it.
    wstatus = diagnose(out[0], testCase->output, error); // if child process ran successfully, diagnose on the output. Reusing wstatus, as it is no longer necessary
    close(out[0]); // close the out stream, as we are done with it
    return wstatus;
  } else { // child process
    dup2(in[0],STDIN_FILENO); // duplicate the read side of the `in` pipe onto `stdin`
    dup2(out[1],STDOUT_FILENO); // duplicate the write side of the `out` pipe onto stdout
    close(in[1]); // no need to write into `stdin`
    close(out[0]); // no need to read from `stdout`
    wstatus = execl(executable, executable, (char *) NULL); // start the test executable with no args. If it fails, exit returning the error code
    exit(wstatus);
  }
}

int diagnose(int result, const char *expected, char **diag) { // check if output is correct, and return reasonable diagnosis
  int i = 0; // iterator through the result and expected
  char c; // character off the stream
  int lineNum = 1; // what line are we on
  int lineStartPos = 0; // position of the start of the current line
  char *lineFrag; // fragment of a line for error output
  while(1) {
    
    if(read(result, &c, 1) == 0) { // read byte, if no bytes are read we're at the end
      if(expected[i] == '\0') { // if we've reached both ends of files successfully
        return 0; // diagnosis: successful
      } else {
        asprintf(diag,"Program output was insufficiently long.\n"); // tell problem
        return -1; // output failed test
      } 
    }
    #ifdef CRUM_DEBUG
    printf("Character: %c\n",c);
    #endif
    if(c != expected[i]) { // if the character is discrepant
      lineFrag = malloc(sizeof(char) * (i - lineStartPos + 2)); // allocate a string large enough to hold our line fragment
      strncpy(lineFrag, expected + lineStartPos,  sizeof(char) * (i - lineStartPos + 1)); // copy the expected fragment into `lineFrag`
      lineFrag[i - lineStartPos + 1] = '\0'; // null-terminate the string
      asprintf(diag,"Discrepancy was found at character %d, on line %d. Line up to the error:\n%s\nDiscrepant character: %c\nExpected character: %c\n",
        i + 1, lineNum, lineFrag, c, expected[i]);
      free(lineFrag); // free the allocated string
      return -1;
    } 
    #ifdef CRUM_DEBUG
    else { puts("Character accepted."); }
    #endif
  if(expected[i] == '\n') {
      lineNum++; // new line. This will make it behave a little odd when the error is a missing newline
      lineStartPos = i + 1; // the next character is the first in the line.
    }
  i++;
  }
}

int swrite(int fd, const char *s) {
  int i = 0; // iterator
  #ifdef CRUM_DEBUG
  puts("Beginning the write loop.");
  #endif
  while(s[i] != '\0') {
    #ifdef CRUM_DEBUG
    printf("Writing char %c into input file.\n", s[i]);
    #endif
    write(fd, s + i++, sizeof(char));
  } // write bytes until we hit the null terminator. Do not write the terminator
  #ifdef CRUM_DEBUG
  puts("Exiting the write loop");
  #endif
  return i;
}
