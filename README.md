##### crumtest
Everyone knows that testing CS assignments is hard. You output needs to be character-perfect, and the human eye is pretty bad at ascertaining that. However, computers are
quite good at doing tedious things for us, so now we have `crumtest`, named in honor of the Purdue professor who inspired this effort.

## Usage
Run `crumtest` from the command line (obviously needs to be available in PATH somehow) with the executable as the first argument, and the test file as the second.
For example, to test my new program `a.out`, where my tests are saved in `crumtests.txt`, I would type:
    crumtest a.out crumtests.txt
This could be futher simplified by use of a `Makefile` or somesuch. The program will give output for each successful test case, and give a diagnostic for failed tests.
For now, it will attempt to continue running if a test fails, so you should expect output for every test.

# Test Files
The program, of course, needs to know what inputs to give your program and what output to expect. This is configured in a **test file**, using simple syntax.

Every test case is composed of input and output. In the test file, specify the input as a string enclosed in square braces, and the output as a following string enclosed
in curly braces. For example, if I want my program to take no input, and outputm "Hello World!", then somewhere in my test file I would add
    []
    {Hello World!}
Whitespace in-between the bracket and curly-brace expressions will be ignored. To add another test case, simply add another set of bracket-and-curly-brace strings to be
tested. 
