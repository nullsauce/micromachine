# <img src="docs/images/micromachine-logo.png" width="640" height="320" />

Micromachine aims to be an cycle-**innacurate** armv6-m emulator that can be used to simulate
microcontrollers at the application level.


### Build status

branch | status
-------|--------
master |![](https://github.com/flavioroth/micromachine/workflows/MicroMachine%20CI/badge.svg?branch=master)


### Structure

`micromachine` contains the emulation code. The most part of it is coded in headers
 in order to minimize the number of compilation unit and allow optimizations to take place.

* :file_folder: [**test**](test/) contains all emulation code tests.

* :file_folder: [**termdbg**](termdbg/) contains a rudimentary terminal UI application used for
developing and debugging the emulator.

* :file_folder: [**vm**](vm/) contains the command-line app (vm) that can execute armv6-m ELF binaries

* :file_folder: [**sdk**](sdk/) contains a standalone cmake project which acts as a poor man's
toolchain for now. Note that the sdk contains its own integration tests.

### Build targets

The CMake project will generate a lot of not-so-useful targets. Here's a list of targets that are important to know:

* **all_tests** Runs all the emulation code tests. Note that this target will appear as a unit test in CLion.

* **sdk** Builds the SDK and examples and runs the integration tests.

* **termdbg** Builds the terminal UI based debugger. The debugger takes a micromachine ELF binary as argument.


### Using CLion

1) Open the project as usual with CLion by selecting the root CMakeLists.txt
2) Import the code style settings located in `.idea/codeStyleSettings.xml`

#### Working on termdbg from CLion

##### Launching from CLion

**termdbg** wont diplay properly in CLion's terminal. A possible workaround is to edit the target launch options so that your terminal of choice is launched instead.

In the `Executable` input, choose `Select other...` and enter the full path of oyur gnome terminal (usually `/usr/bin/gnome-terminal`)

In the `Program arguments` input, enter:

    --geometry 120x80 -- $CMakeCurrentProductFile$ $CMakeCurrentGenerationDir$/sdk/examples/hello_world/hello_world

* `$CMakeCurrentProductFile$` points to the **termdbg** binary being built.
* `$CMakeCurrentGenerationDir$` points to the current cmake binary dir (`cmake-build-*`)

This will launch **termdbg** in a gnome-terminal when you run the target from CLion, but won't work when debugging.

##### Debugging from CLion

Launch a debug build of **termdbg** with the CLion target as normal then manually attach the debugger to the termdbg manually via the `Run` menu.

You might get a **`Operation not permitted`** caused by a lack of ptrace permission. This can be resolved by running the following command:

    sudo echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

For more info about Yama see https://www.kernel.org/doc/Documentation/security/Yama.txt


##### Debugging with termdbg

TODO

<a href="https://asciinema.org/a/319983">
<img src="https://asciinema.org/a/319983.png" width="269" height="180" />
</a>

### Command-line

This project is a standard CMake project and should work fine using the command-line only.
See the CI scripts for examples.





### Documentation resources

* [ARM® v6-M Architecture Reference Manual](https://static.docs.arm.com/ddi0419/d/DDI0419D_armv6m_arm.pdf)
* [ARM Infocenter](http://infocenter.arm.com/help/index.jsp)





