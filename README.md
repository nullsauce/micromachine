# Micromachine

Micromachine aims to be an cycle-**innacurate** armv6-m emulator that can be used to simulate
microcontrollers at the application level.


### Build status

![](https://github.com/flavioroth/micromachine/workflows/MicroMachine%20CI/badge.svg?branch=master) **master**


### Structure

`micromachine` contains the emulation code. The most part of it is coded in headers
 in order to minimize the number of compilation unit and allow optimizations to take place.

* :file_folder: `test` contains all emulation code tests.

* :file_folder: `termdbg` contains a rudimentary terminal UI application used for
developing and debugging the emulator.

* :file_folder: `vm` contains the command-line app (vm) that can execute armv6-m ELF binaries

* :file_folder: `sdk` contains a standalone cmake project which acts as a poor man's
toolchain for now. Note that the sdk contains its own integration tests.

### Build targets

The CMake project will generate a lot of not-so-useful targets. Here's a list of targets that are important to know:

* :hammer: `all_tests` Runs all the emulation code tests. Note that this target will appear as a unit test in CLion.

* :hammer: `sdk` Builds the SDK and examples and runs the integration tests.

* :hammer: `termdbg` Builds the terminal UI based debugger. The debugger takes a micromachine ELF binary as argument.


### Using CLion

1) Open the project as usual with CLion by selecting the root CMakeLists.txt
1) Import the code style settings located in `.idea/codeStyleSettings.xml`

### Command-line

This project is a standard CMake project and should work fine using the command-line only.
See the CI scripts for examples.


### Documentation resources

* [ARM® v6-M Architecture Reference Manual](https://static.docs.arm.com/ddi0419/d/DDI0419D_armv6m_arm.pdf)
* [ARM Infocenter](http://infocenter.arm.com/help/index.jsp)





