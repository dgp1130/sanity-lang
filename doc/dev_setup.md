# Dev Setup

To build and run Sanity for development purposes, the following steps are needed.

## Install LLVM

Sanity uses LLVM as a dependency. Currently, LLVM does not automatically install itself and must be manually built from
source. Follow the instructions [here](https://github.com/abenkhadra/llvm-pass-tutorial) to compile LLVM from source.
Use the default install directory and Sanity should automatically pick it up. Currently, Sanity has only been tested
with LLVM version 3.9.1, so that is version which should be downloaded.

## Clone Sanity

Sanity is hosted on GitHub if you didn't notice, which, *surprise*, uses Git.

```bash
$ git clone https://github.com/dgp1130/sanity-lang
$ cd sanity-lang
```

## Build and Run Sanity

Sanity uses the CMake build system. CMake can be installed using your friendly neighborhood package manager. To actually
build the compiler, run:

```bash
$ cmake --build . --target Sanity
```

Sanity is built in `build/bin/Sanity`. It must be executed with the filename of the Sanity source code to compile. Hence
it can be easily run with:

```bash
$ build/bin/Sanity hello.sane
```

## Test Sanity

Tests can be built using the following CMake command.

```bash
$ cmake --build . --target SanityTest
```

Sanity uses [GoogleTest](https://github.com/google/googletest) as its test framework. CMake will automatically download
the GoogleTest dependency from source and build as part of the repo. This should happen automatically without requiring
any special set up. Tests can run with:

```bash
$ build/bin/SanityTest
```

## IDE

Currently the only editor that has been used to develop Sanity is CLion (Intellij), so that's the only one with
experience. It should be possible to import the project by the
[CMakeLists.txt](https://github.com/dgp1130/sanity-lang/blob/master/CMakeLists.txt) file. Once the project is loaded,
Run configurations for running and testing the compiler should be automatically loaded with the names "Sanity" and
"SanityTest". If it automatically imports other configurations (like "parser", "exceptions", "lexer", ...) then those
can be deleted. Need to look into how to stop that.