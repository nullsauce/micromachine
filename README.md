# Project

## Build status

`master` ![](https://github.com/flavioroth/micromachine/workflows/MicroMachine%20CI/badge.svg?branch=master)

micromachine aims to be an armv6-m emulator that can be used to simulate
microcontrollers at the application level.

### Files
`micromachine` contains the cpu emulation code. Most of it is header-only at the
moment. This might change in the future. This subproject generates a static
c++ library containing all the emulation logic.

`termdbg` contains a terminal UI application used for
developing and debugging the cpu implementation.

`vm` contains the command-line app (vm) that can execute armv6-m executables

`sdk` contains a standalone cmake project which acts as a poor man
toolchain for now.

`test` contains all the tests.

# Howto

## Using an IDE

### CLion

1) Open the project as usual with CLion by selecting the root CMakeLists.txt
1) Import the code style settings located in `.idea/codeStyleSettings.xml`




