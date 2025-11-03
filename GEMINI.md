# GEMINI.md

## Project Overview

This project is a C++-based traffic monitoring and sizing tool designed to analyze packet sizes across different operating systems. It has been validated on RHEL 8.7 and Windows 10. The tool is built using CMake and a custom shell script, `processMake.sh`.

The project is structured as follows:

*   **`Apps/`**: Contains the main application entry point (`SizingMain.cpp`).
*   **`Sources/`**: Contains the core logic of the application, including different controllers for each operating system.
*   **`Models/`**: Contains data models and file parsers.
*   **`Services/`**: Contains services used by the application.
*   **`Headers/`**: Contains the header files for the C++ code.
*   **`Settings/`**: Contains configuration files for the project, including CMake scripts and JSON settings.
*   **`Tests/`**: Contains unit tests for the project.
*   **`Vendors/`**: Contains third-party dependencies.

## Building and Running

The project is built using the `processMake.sh` script, which automates the CMake configuration and build process.

### Build Instructions

1.  **Install Dependencies:** The `processMake.sh` script will automatically install the required dependencies listed in `Settings/.Json/globalDependencies.json` by running the `Shells/installVendor.sh` script.
2.  **Run the build script:**
    ```bash
    ./processMake.sh
    ```
    This will create a `build/` directory and compile the project. The executables will be placed in the `Bin/` directory.

### Build Targets

The `processMake.sh` script supports different build targets for different operating systems:

*   **Linux:** The script will automatically detect the Linux distribution (RHEL or CentOS) and build the appropriate version.
*   **Windows (MinGW):** The script will build two versions of the application:
    *   One for Npcap (`OPERATING_SYSTEM=1`)
    *   One for WinDivert (`OPERATING_SYSTEM=1.1`)

### Configuration

The application can be configured by editing the `Settings/Json/SizingMain.json` file. This file allows you to specify the ports to intercept for traffic monitoring.

### Running the Application

The executables are located in the `Bin/` directory. On Windows, the application must be run with administrator privileges.

## Development Conventions

*   **Build System:** The project uses CMake for its build system. The main `CMakeLists.txt` file is modular and includes other `.cmake` files from the `Settings/.Cmake` directory.
*   **Operating System Abstraction:** The application uses a controller-based architecture to abstract the operating system-specific code. The `SizingMainController` class in `Sources/SizingControllers/SizingMainController.cpp` is the main entry point, and it delegates to the appropriate controller for the current operating system.
*   **Testing:** The project includes a `Tests/` directory for unit tests. The tests can be enabled by setting the `BUILD_TEST` CMake variable to `ON`.
