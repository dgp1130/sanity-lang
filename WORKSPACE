# Note: Make sure this stays in sync with the WORKSPACE_NAME value in //:build_vars.bzl.
# Unfortunately, we can't load the Skylark module until after the workspace() call.
workspace(name = "sanity")

load("//:build_vars.bzl", "WORKSPACE_NAME")
load("//build_defs:llvm.bzl", "llvm")

llvm(
    name = "llvm",
    workspace_name = WORKSPACE_NAME,
)

git_repository(
    name = "gflags",
    commit = "46f73f88b18aee341538c0dfc22b1710a6abedef",  # 2.2.1
    remote = "https://github.com/gflags/gflags.git",
)

new_git_repository(
    name = "gtest",
    build_file = "gtest.BUILD",
    commit = "40f82ce56a4b416aa4631e48d1d07377793b18ee",  # ~1.8.0
    remote = "https://github.com/google/googletest.git",
)
