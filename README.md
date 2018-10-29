# Project

micromachine aims to be an armv6-m emulator that can be used to simulate
microcontrollers at the application level.

### Files
`lib` contains the cpu emulation code. Most of it is header-only at the
moment. This might change in the future. This subproject generates a static
c++ library containing all the emulation logic.

`debugger` contains a graphical application (QML based) used for
developing and debugging the cpu implementation.

`run` contains a small command-line app that execute the elf executable
passed as argument.

`sdk` contains a standalone cmake project which acts as a poor man
toolchain for now.

`test` contains all the tests.

# Howto

## Using an IDE

### Qt Creator

1) Download and install Qt 5.9 or higher
2) In Qt Creator:
    1) File -> Open File or Project -> Select the root CMakeLists.txt file

    2) If you're only interested in building the graphical UI, click on
       Projects Icon -> in Build section, scroll down to **Build Steps**,
       expand *details* uncheck everything except `visual_debugger`.

       Then on Building icon (the one with the computer monitor) make sure
       `visual_debuger` is selected under *Run*.

### CLion

1) Import the code style settings located in `.idea/codeStyleSettings.xml`
2) See [Using the command line](#using-the-command-line)

### Using the command line

1) Download and install Qt 5.9 or higher
2) Update `env.cmake` with your Qt path.
2) Invoke cmake as usually.


