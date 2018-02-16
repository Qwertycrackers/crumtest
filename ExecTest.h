#ifndef _CRUMEXECTEST_
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "Test.h"

int execTest(const char *executable, struct Test *testCase, char **error);

#define _CRUMEXECTEST_
#endif


