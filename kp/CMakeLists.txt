cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(kp LANGUAGES CXX)


add_executable(main ./src/main.cpp ./src/binary_allocator.h ./src/freeblock_allocator.h ./src/allocator.h)
