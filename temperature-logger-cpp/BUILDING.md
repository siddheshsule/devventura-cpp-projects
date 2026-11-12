# Building and testing (C++)

Every Devventura C++ project builds the same way.

## What you need

- A C++20 compiler: GCC 11+, Clang 14+ or Visual Studio 2022 (MSVC 19.30+)
- CMake 3.20 or newer
- Git (recommended)

Check with `cmake --version` and `g++ --version` (or `clang++ --version`).

## First run

```sh
git init && git add . && git commit -m "Starter code"   # start a history, like on the job
cmake -S . -B build
cmake --build build
ctest --test-dir build -L m1 --output-on-failure
```

The tests fail at first. That's expected: making them pass is your job.

**Windows with Visual Studio:** run the same commands in the "Developer PowerShell for VS 2022",
and add `-C Debug` to `ctest` (Visual Studio builds several configurations).

## Running tests

Each milestone has its own label:

```sh
cmake --build build && ctest --test-dir build -L m1 --output-on-failure   # milestone 1
cmake --build build && ctest --test-dir build -L m2 --output-on-failure   # milestone 2
ctest --test-dir build --output-on-failure                                # everything
```

You may change anything in `src/` and `include/`. Don't change the tests to make them pass.

## Folders you don't need to touch

| Path                   | What it is                                             |
| ---------------------- | ------------------------------------------------------ |
| `cmake/`               | Shared build setup (compiler flags, test registration) |
| `tests/test_main.cpp`  | Starts the test runner                                 |
| `third_party/doctest/` | The doctest test framework (MIT license)               |
