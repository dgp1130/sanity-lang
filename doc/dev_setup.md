# Dev Setup

To build and run Sanity for development purposes, the following steps are needed.

## Prerequisites

Currently, the Sanity compiler is only supported on Linux, mainly because that's the system I'm using to develop (Ubuntu
16.04) and because I'm too lazy to make it support Windows. At time of writing, the only non-portable feature is that it
defaults to a hard coded `/dev/stdin` when not given an input file. Also it has a hard-coded dependency on LLVM's Linux
pre-built binaries. This can and should support Windows, I'm just not really bothering with it at the moment.

You probably want to install Git, though you can download tarballs of the repo instead if you really want to.

[Bazel](https://bazel.build/) is the build tool used by Sanity. It handles all dependency management. Simply install
Bazel and you have everything you need. To learn more about how Bazel works see the
[Bazel Overview](https://docs.bazel.build/versions/master/bazel-overview.html). The
[example C++ project](https://docs.bazel.build/versions/master/tutorial/cpp.html) is also a great way to get started.

## Clone Sanity

Sanity is hosted on GitHub if you didn't notice, which, *surprise*, uses Git.

```bash
$ git clone https://github.com/dgp1130/sanity-lang
$ cd sanity-lang
```

## Build and Run Sanity

The simple `hello.sane` file can be executed with:

```bash
$ bazel run //:hello
```

### Build Definitions

The `sanity_binary()` Bazel definition is in [sanity.bzl](../build_defs/sanity.bzl). This accepts a Sanity source file
and outputs the compiled binary which executes it. Note that any dependencies need to be included, so if you want to
call an external function, it needs to be included there.

## Test Sanity

All tests can be executed with:

```bash
$ bazel test //...
```

Add `--test_output=errors` to immediately see any failing tests. You can also create a file at `~/.bazelrc` with the
content:

```
test --test_output=errors
```

So the flag is automatically added to all runs of `bazel test`.