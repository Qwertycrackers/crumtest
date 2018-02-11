#include "ExecTest.h"

int diagnose(FILE *result, const char *expected, char **diag);

int execTest(const char *executable, struct Test *testCase, char **error) {
  int in[2]; // pipe for input
  int out[2]; // pipe for output
  int pid; // process id returned from the fork
  int wstatus; // receiver for the status of child program

  pipe(in); // create the pipe mocking `stdin`
  pipe(out); // create the pipe mocking `stdout`
  pid = fork(); // we forked babeeee

  if(pid == -1) { // something went wrong
    *error = "Failed to successfully fork process.\n"; // make `error` meaningful
    return -2; // error code
  }
  if(pid == 0) { // fork successful, parent process
    fclose(in[0]);
    fclose(out[1]); // don't need to write to `out` or read from `in`
    fputs(testCase->input, in[1]); // feed the input into the child program
    do {
      wait(&wstatus); // wait for a change in child process
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
    fclose(in[1]); // close the write side of `in`, as we are done with it.
    wstatus = diagnose(out[0], testCase->output, error); // if child process ran successfully, diagnose on the output. Reusing wstatus, as it is no longer necessary
    fclose(out[0]); // close the out stream, as we are done with it
    return wstatus;
  } else { // child process
    dup2(in[0],stdin); // duplicate the read side of the `in` pipe onto `stdin`
    dup2(out[1],stdout); // duplicate the write side of the `out` pipe onto stdout
    fclose(in[1]); // no need to write into `stdin`
    fclose(out[0]); // no need to read from `stdout`
    wstatus = execvp(executable, NULL); // start the test executable with no args. Reusing wstatus
    exit(wstatus); // if we get here, `execvp` failed, so exit and return the error to our parent (probably -1)
  }
}

int diagnose(FILE *result, const char *expected, char **diag) { // check if output is correct, and return reasonable diagnosis
  int i = 0; // iterator through the result and expected
  char c; // character off the stream
  int lineNum = 1; // what line are we on
  int lineStartPos = 0; // position of the start of the current line
