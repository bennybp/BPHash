# BPHash - A C++11 Hashing Library

**BPHash** is a C++ library designed for easy hashing of
data types commonly encountered in C++.

The main documentation is found at (...)


## General Features

BPHash can be used to hash:

- C++ STL containers
- Enumerations
- Raw pointers and arrays
- Smart pointers
- Custom classes and structures

In addition, **BPHash** also contains a structure that can
be used in place of std::hash in containers such as `unordered_set`

Detection of the ability to be hashed is generally done at compile time,
reducing the number of possible errors at runtime.


## Requirements

All that is required is a C++11 compiler and CMake v3.0.2 or above
(https://cmake.org)


## Limitations

BPHash is designed to be consistent between runs of a program. It is
not designed to be consistent between different compilations nor across
compilers or platforms. If compiling with the same compiler and standard
library, the hashes should be consistent.

The reason for this: while the underlying hash algorithms are compiler
and platform-agnostic, part of the hashing using the mangled type
name. Therefore, using a different compiler or standard library will
result in different hashes.

Also, the current hashing algorithms are not designed for use as a
cryptographic hash.


## License


