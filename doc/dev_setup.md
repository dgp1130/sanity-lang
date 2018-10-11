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

## IDEs

Currently the only tested editor is CLion (Intellij IDEA). This provides support for Bazel through a plugin. Other IDEs
can theoretically be used, but your mileage may vary. Ideally they should support C/C++ and Bazel. The rough steps to
set up CLion are as follows (everything here is pretty standard Bazel with Intellij, just a little scary if you're not
familiar with them):

1. Install [CLion](https://www.jetbrains.com/clion/). This is a paid IDE, but it's definitely worth the money and
students can often get it for free.
1. Install the Bazel plugin by following the instructions [here](https://ij.bazel.build/docs/bazel-plugin.html).
1. Restart CLion so the plugin is enabled.
1. Follow the instructions [here](https://ij.bazel.build/docs/import-project.html) to import the project configuration
into CLion.
    1. Click "Import Bazel Project" at the CLion splash screen.
    1. For the workspace root, choose the directory you cloned Sanity to (the directory immediately containing the
    `WORKSPACE` file).
    1. For the "Project View", there is already a [bazelproject](/idea/bazelproject) file checked in. Just choose
    "Import from workspace" and then enter "idea/bazelproject" to pick that file relative to the root workspace
    directory.
    1. Finally, pick the name of the project to display in CLion as well as a location to store project data (ultimately
    neither option is important, pick whatever you want or just stick with the defaults).
        * The [bazelproject](/idea/bazelproject) file contents should be displayed. No need to edit anything.
        * Make sure "Use shared project view file" is checked.

CLion should open with everything configured. Look around some C++ code and BUILD files to make sure everything is
working correctly. This should also automatically load the Run Configurations. In the top-right of the UI, there should
be a play button with a drop down to select a target. Simply press play to run that target. If you see "Edit
Configurations" with no other options, then click "Edit Configurations" and see if there are existing targets which need
to be selected. Might be a weird bug in the Intellij UI that it doesn't display them correctly the first time.
