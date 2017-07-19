# Global Approx Check

### Build the pass:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

    or

    $ mkdir build
    $ cd build
    $ cmake ..
    $ cd ..
    $ make

### compile the test
    $ clang test.c -o test

### to run test
    $ ./test

### build IR
    $ clang -O0 -emit-llvm test.c -c -o test.bc

### look at LLVM IR
    $ llvm-dis < test.bc | less

### run the pass
    $ opt -load build/GlobalApproxCheck/libGlobalApproxCheck.so -GlobalApproxCheck -disable-output test.bc

    or

    $ opt -load GlobalApproxCheck/libGlobalApproxCheck.so -GlobalApproxCheck -disable-output test.bc
