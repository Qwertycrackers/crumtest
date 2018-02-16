cmake_minimum_required(VERSION 3.5.1)
project(crumtest)
add_executable(crumtest main.c Test.c ExecTest.c)

set(CMAKE_C_FLAGS_DEBUG "-DCRUM_DEBUG")
