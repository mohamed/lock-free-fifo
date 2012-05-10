lock-free-fifo
==============
This repository contains a lock-free implementation of FIFO communication for Synchronous Dataflow (SDF). This implementation is based on an original implementation by Hristo Nikolov and has been modified to support token sizes different from 1. This code is distributed under the license shown in LICENSE.md

Using it
========
Simply include `lock-free-fifo.h` and `lock-free-fifo.c` in your project. To see how to use them, look at the test cases `test1.c` and `test2.c`


