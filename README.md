# BPHash - A Hashing Library for C++11

**BPHash** is a C++ library designed for easy hashing of
data types commonly encountered in C++.

The main documentation is found at https://www.bennyp.org/programming/bphash


## General Features

**BPHash** can be used to hash:

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

**BPHash** is designed to be consistent between runs of a program. It is
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

Copyright (c) 2016, Benjamin Pritchard <ben@bennyp.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


