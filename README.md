# Visual Studio Code project template for nRF52840

This repository contains the configuration files for Visual Studio Code and the source code for a demo project. The demo project relies on external definitions only and uses custom start-up code to provide full control over every aspect of the application.

The project is tailored to the nRF52840 microcontroller, but can easily be adjusted to any other chip of the nRF52 series. Similarly, the pin configuration and debugging tools assume an nRF52840-DK development board.

The setup has been tested on Arch Linux and should work on other Linux distributions (Ubuntu) without problems. All the tools are available for Windows, but might require a different procedure and changes to some configuration files.

## Getting started

## Building on Linux
 - If you don't have it yet, install `make` from your distribution's repository (`build-essential`)
 - Install the `GNU Arm Embedded Toolchain` from your distribution's repository (`arm-none-eabi-gcc`) or from [the official website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
 - Install the `SEGGER Software` by downloading and running the installer from [the official website](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
 - Install the [nRF-Command-Line-Tools](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Command-Line-Tools/Download) following the official instructions
 - Download and extract the [nRF5 SDK](https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download) (you don't need a SoftDevice) to a path on your machine.
 - Linux only: Install `clang-format` from your distribution's repositories
 - Install Visual Studio Code
 - Install the `Cortex-Debug`, `Clang-Format` and `Serial Monitor 2` extensions from the Visual Studio Code marketplace
 - Clone this repository: `git clone https://github.com/geissdoerfer/nrf52-vscode`
 - Start VSCode and open the folder where you cloned the code via `File->Open Folder...`
 - Change the `sdkRoot` and `gnuInstallRoot` variables in `.vscode/settings.json` to the paths where you installed the SDK and toolchain
 - Set the `opersys` variable in `.vscode/settings.json` to `Linux`
 - In Visual Studio Code, press `Ctrl`+`Shift`+`p`, type `Open Keyboard Shortcuts` and press `Enter`, insert the following keybindings and save the file:

```
    {
        "key": "ctrl+alt+b",
        "command": "workbench.action.tasks.runTask",
        "args": "build"
    },
    {
        "key": "ctrl+alt+c",
        "command": "workbench.action.tasks.runTask",
        "args": "clean"
    },
    {
        "key": "ctrl+alt+u",
        "command": "workbench.action.tasks.runTask",
        "args": "flash"
    },
```

## Building on Windows
 - To run makefile in windows, install `Cygwin` by downloading and running the installer from [the official website](https://cygwin.com/).
   Provide the following path (pathto/cygwin/bin) in your system variables.
 - Install the `GNU Arm Embedded Toolchain` from [the official website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
 - Install the `SEGGER Software` by downloading and running the installer from [the official website](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
 - Install the [nRF-Command-Line-Tools](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Command-Line-Tools/Download) following the official instructions
 - Download and extract the [nRF5 SDK](https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download) (you don't need a SoftDevice) to a path on your machine.
 - Install Visual Studio Code
 - Install the `Cortex-Debug`, `Clang-Format` and `Serial Monitor 2` extensions from the Visual Studio Code marketplace
 - Clone this repository: `git clone https://github.com/geissdoerfer/nrf52-vscode`
 - Start VSCode and open the folder where you cloned the code via `File->Open Folder...`
 - Change the `sdkRoot` and `gnuInstallRoot` variables in `.vscode/settings.json` to the paths where you installed the SDK and toolchain
 - Set the `opersys` variable in `.vscode/settings.json` to `Win32`
 - In Visual Studio Code, press `Ctrl`+`Shift`+`p`, type `Open Keyboard Shortcuts` and press `Enter`, insert the following keybindings and save the file:

```
    {
        "key": "ctrl+alt+b",
        "command": "workbench.action.tasks.runTask",
        "args": "build"
    },
    {
        "key": "ctrl+alt+c",
        "command": "workbench.action.tasks.runTask",
        "args": "clean"
    },
    {
        "key": "ctrl+alt+u",
        "command": "workbench.action.tasks.runTask",
        "args": "flash"
    },
```

## Usage

Try building the project with `Ctrl`+`Alt`+`b`. Upload the code to the hardware with `Ctrl`+`Alt`+`u`. You should see LED1 on the development board blinking.
Connect with a serial monitor (e.g., `minicom` on Linux, `Putty` on Windows or the serial monitor from the VSCode extension) at a baudrate of 1000000 B/s. You should see the output from the main loop.
Finally, set a breakpoint somewhere in the code and press `F5` to start stepping through the code.
