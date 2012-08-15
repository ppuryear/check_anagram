check_anagram
=============
This repository contains a small tool that allows you to check whether two
strings are anagrams.

Currently, there are three implementations: one in Python (`check_anagram.py`),
one in C++ (`check_anagram.cc`), and one in Go (`check_anagram.go`). All are
invoked using roughly the same command-line syntax, so which one you use is a
matter of preference.

Python
------
The Python implementation requires Python 3 and can simply be run as a script
(run `check_anagram.py -h` for details).

C++
---
To build the C++ implementation, you need the following installed on your
system:

*   [ninja](http://martine.github.com/ninja/) (a small build system similar to
    make)
*   [boost](http://www.boost.org/) >= 1.50

Run `ninja` in the directory containing `build.ninja` to build the executable.

Go
---
The [Go](http://golang.org) implementation can be built via
`go build check_anagram.go` or similar.

