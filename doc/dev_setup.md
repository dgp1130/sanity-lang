# Dev Setup

To build and run Sanity for development purposes, the following steps are needed.

## Prerequisites

Currently, the Sanity compiler is only supported on Linux, mainly because that's the system I'm using to develop (Ubuntu
16.04) and because I'm too lazy to make it support Windows. At time of writing, the only non-portable feature is that it
defaults to a hard coded `/dev/stdin` when not given an input file. This can and should support Windows, I'm just not
really bothering with it at the moment.

## Install Dependencies

The Sanity compiler has a few dependencies which need to be manually installed. Someday I hope to clean this up so they
are automatically built without extra manual steps.

### Install LLVM

Currently, LLVM does not automatically install itself and must be manually built from source. Follow the instructions
[here](https://github.com/abenkhadra/llvm-pass-tutorial) to compile LLVM from source. Use the default install directory
and Sanity should automatically pick it up. Currently, Sanity has only been tested with LLVM version 3.9.1, so that is
the version which should be downloaded.

### Install GFlags

GFlags is used as an argument parsing library. Follow the instructions
[here](https://github.com/gflags/gflags/blob/master/INSTALL.md) to build GFlags from source and install it to the
default directory, which should automatically be picked up by Sanity when it builds. Currently Sanity has only been
tested with version 2.2.1, use other versions at your own risk.

## Clone Sanity

Sanity is hosted on GitHub if you didn't notice, which, *surprise*, uses Git.

```bash
$ git clone https://github.com/dgp1130/sanity-lang
$ cd sanity-lang
```

## Build and Run Sanity

Sanity uses the CMake build system. CMake can be installed using your friendly neighborhood package manager. To actually
build the compiler, first initialize CMake:

```bash
$ cmake .
```

This only needs to be done once, then each time you want to build the compiler, run:

```bash
$ cmake --build . --target Sanity
```

Sanity is built in `build/bin/Sanity`. It must be executed with the filename of the Sanity source code to compile or it
will read from stdin. A sample Sanity source code file is in the root directory of the repository for testing. As an
example, it can be run with:

```bash
$ build/bin/Sanity --input hello.sane
```

Currently, running Sanity directly simply outputs the LLVM IR which can execute it. To actually run this code, it needs
to be compiled with its dependencies, currently just the stdlib it uses. This can be quite complex to do manually, so
there is a [run.sh](../run.sh) script to do this for you. Simply execute:

```bash
$ ./run.sh hello.sane
```

Any program or logic which cannot currently be expressed in Sanity, can be written in a C file under `stdlib/`. That
will be linked to the Sanity program and can be accessed through an extern. This is a useful escape hatch when trying to
create functionality which is not yet entirely supported by Sanity.

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