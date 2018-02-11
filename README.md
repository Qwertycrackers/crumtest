# crumtest
Everyone knows that testing CS assignments is hard. You output needs to be character-perfect, and the human eye is pretty bad at ascertaining that. However, computers are
quite good at doing tedious things for us, so now we have `crumtest`, named in honor of the Purdue professor who inspired this effort.

## Usage
Run `crumtest` from the command line (obviously needs to be available in PATH somehow) with the executable as the first argument, and the test file as the second.
For example, to test my new program `a.out`, where my tests are saved in `crumtests.txt`, I would type:
    crumtest a.out crumtests.txt
This could be futher simplified by use of a `Makefile` or somesuch. The program will give output for each successful test case, and give a diagnostic for failed tests.
For now, it will attempt to continue running if a test fails, so you should expect output for every test.

### Test Files
The program, of course, needs to know what inputs to give your program and what output to expect. This is configured in a **test file**, using simple syntax.

Every test case is composed of input and output. In the test file, specify the input as a string enclosed in square braces, and the output as a following string enclosed
in curly braces. For example, if I want my program to take no input, and output "Hello World!", then somewhere in my test file I would add
    []
    {Hello World!}
Whitespace in-between the bracket and curly-brace expressions will be ignored. To add another test case, simply add another set of bracket-and-curly-brace strings to be
tested. As far as a know, there are no assignments which include input with brackets or output with curly braces. If there ever are, this program will need a more 
intelligent delineation scheme. For now, however, this will be fine.

## How to Get It
Seeing as `crumtest` is a simple open-source package, there are multiple ways to get the software.

### Downloading the Compiled Binary
If you're using `crumtest`, you're probably in a first-year programming class. As such, the easiest way to get it would be to download the binary release from the "Releases" tab on Github. Place the executable somewhere it can be found on the development machine (likely on your network drive, in the folder with your code), and run it as
described in the **Usage** section.

### Building from Source
It is also, of course, possible and easy to build `crumtest` from source. This will be necessary if you wish to run the program on any architecture other than x86\_64.
To do this, simply clone the repository and run
    make all
This should generate a new `crumtest` executable. It can then be run as described in the **USAGE** section.

### Cross-Platform Support
`crumtest` is entirely dependent on the POSIX standard and Linux system calls. As such, it is completely incompatible with any OS other than Linux. The binaries offered in the **RELEASES** tabs are x86\_64, but this is not a requirement, and the program could entirely be compiled for other architectures. The Purdue Guru server is x86\_64
though, so this is unlikely to be a common necessity.

## How Does It Work?
`crumtest` works by use of `fork()` and `execve()`. The `execve()` system call essentially transforms the current process into the one pointed to by the argument of the
function. The `fork()` call simply causes the current program to be duplicated to another process, leaving some information as to whether the program is the parent or child
process. The child process redirects `stdin` and `stdout` to pipes pointing back to the parent process, and then runs the testing executable.

In the parent process, the test input is piped in to the mock-`stdin`, and the parent then waits for the child to finish execution. The output is read from the mock-`stdout` and compared for errors, and any issues are passed up to the calling function. The main function catches these and produces nice output.

The test cases are generated from a user-given file with simple delineation syntax. The input string is between square brackets(`[]`) and the expected output follows
between curly braces(`{}`). Any other characters are ignored.
