ZooReaders
==========

Some code for reading ZooNtuples adding convenience, trying to avoid code
duplication. Providing commonly needed code. etc.

usage
=====

* symbolic link to your Phys/ZooWriter/Zoo directory in the ZooReaders main
  directory is required.
* before running executables "source init.sh" is required
* for developers: set a pre-commit hook which runs doxygen and have a look at http://rickfoosusa.blogspot.de/2011/10/howto-use-doxygen-with-github.html

todo
====

* Follow this example: http://rickfoosusa.blogspot.de/2011/10/howto-use-doxygen-with-github.html
and generate doxygen docu on the fly and ship it!
* logstreams don't use deprecated (as in C++11) auto pointers.
* finish Loopers implementation
* proper makefile for helpers
* example using Loopers

using options
=============

look at ```hello_options.cc```
and run:
* ```./hello_options```
* ```./hello_options -h```
* ```./hello_options -v 34```
* ```./hello_options here some words```
* ```./hello_options -s 0xdeadface```
* ```./hello_options -w 1 -t bla some words```

through the examples
====================

When reading through the examples _please_ don't start just somewhere. Go
through them in the following order.

1. [hello_options.cc] the most readable intro into options I could think of.
2. [hello_logstreams.cc] just putting some noise to the logstreams. play around
   with the -v option. Shows how functions can be called when an option gets
   parsed, both using the value as well as not using it (mind the template of option::myfun).
3. [hello_logstreams_glob.cc] not to introduce code duplication, most of
   ```hello_logstreams.cc``` is exported to the helpers library. This shows how
   you're supposed to use options.
4. [ZooReader_basic.cc] a minimal executalbe close to the old ZooReader
   example.  NB: It shows a check if the branch exists and tells the user all
   available branch names.
5. [ZooReader_options.cc] adding options as in hello_options.cc to the
   ZooReader.
6. [ZooReader_glob_options.cc] putting things together from
   hello_logstreams_glob.cc and ZooReader_options.cc

the ZooGui
==========

This is just a toy project. Two ways of using it:

Firstly, in interactive root:
```
root -l
gSystem->Load("Zoo/libZooROOT.so")
gSystem->Load("ZooFunctors/libZooFunctorROOT.so")
.L ZooGui.cpp+O
ZooGui gui
gui.LoadFile("/path/to/your/ntuple.root")
```

Secondly executed:
```
make ZooGui
./ZooGui </path/to/your/ntuple.root>
```

Thirdly, due to limitations of CINT you cannot actually develope your plot on
runtime. This could be much more awesome with the PyROOT methods shipped with
Phys/ZooWriter/examples. I'm curious if this gets fantastic with cling/clang.
