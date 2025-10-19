# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

---

## ABSTRACT

This document describes the architecture, build procedures, and development guidelines for a C++ network traffic monitoring and sizing tool designed for cross-platform deployment. The system implements platform-specific packet capture mechanisms using libpcap for Linux environments and WinDivert for Windows platforms. The tool identifies device classes supported by DBAEGIS decoders through real-time packet analysis. This documentation follows IEEE technical documentation standards and provides essential information for developers working with the codebase.

**Index Terms**—Network monitoring, packet capture, cross-platform development, CMake build system, libpcap, WinDivert.

---

## I. INTRODUCTION

The native-sizing project consists of two primary C++ applications designed for network traffic analysis across heterogeneous operating systems:

1. **Sizing Application**: Facilitates rapid assessment of packet sizes and identifies device classes supported by DBAEGIS decoders
2. **PacketRedirection Application**: Implements packet forwarding with load balancing capabilities for distributing network traffic across multiple targets

Both applications have been validated on RHEL 8.7 (Red Hat Enterprise Linux) and support cross-platform deployment with Windows 10 (via Msys2 environment).

### A. Purpose and Scope

This documentation provides comprehensive guidance for:
- Building and testing the application across supported platforms
- Understanding the architectural design and component organization
- Configuring runtime parameters for network monitoring
- Extending platform support through the controller pattern implementation

### B. System Requirements

**Development Environment:**
- g++ (GCC) 8.5.0 20210514 or later
- GNU Make 4.2.1 or later
- CMake version 3.20.2 or later
- jq 1.6 (JSON processor)
- GNU Awk 4.2.1
- rsync, unzip, git

**Platform-Specific Requirements:**
- Linux: libpcap library (RHEL 8.7 validated)
- Windows: WinDivert version 2.2.2 (Msys2/MinGW64 environment)

---

## II. BUILD AND DEPLOYMENT PROCEDURES

### A. Standard Build Process

The primary build script (`processMake.sh`) executes the following sequence:

```bash
./processMake.sh
```

**Build Sequence:**
1. Dependency resolution via `Settings/.Json/globalDependencies.json`
2. Creation of runtime directories (`Outputs`, `Logs`, `Bin`)
3. CMake configuration with parameters `-DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug`
4. Binary compilation via `cmake --build build`
5. Platform-specific post-processing (Windows only: WinDivert file deployment)

### B. Clean Build Operation

To remove existing build artifacts and perform a clean compilation:

```bash
./processMake.sh --remove
```

Alternative syntax: `./processMake.sh -r`

### C. Unit Testing

The test execution script (`unitTest.sh`) performs comprehensive unit testing:

```bash
./unitTest.sh
```

**Test Sequence:**
1. Dependency installation
2. CMake configuration with `-DBUILD_TEST=ON` (default setting)
3. Binary compilation
4. Test execution via `ctest --verbose --rerun-failed --output-on-failure`

For clean test builds: `./unitTest.sh --remove`

### D. Version Management

Version incrementation is managed through the package script:

```bash
./package.sh          # Uses '#' separator
./package.sh <arg>    # Uses '-' separator
```

Version information is maintained in `Settings/.version`.

---

## III. SYSTEM ARCHITECTURE

### A. Design Overview

The system implements a factory pattern for platform-specific controller instantiation, enabling compile-time selection of appropriate packet capture mechanisms based on the target operating system.

### B. Controller Pattern Implementation

**Entry Point:** `Apps/Sizing/SizingMain.cpp`

The main function delegates execution to `SizingMainController`, which instantiates platform-specific implementations:

1. **Linux Generic Implementation** (`LinuxSizingMainController`)
   - Activated when `OS_ID` preprocessor macro is undefined
   - Uses standard libpcap interface

2. **Linux CentOS Implementation** (`LinuxCentosSizingMainController`)
   - Activated when `OS_ID` is defined in CMake configuration
   - Provides distribution-specific optimizations

3. **Windows Implementation** (`WindowsSizingMainController`)
   - Activated when `_WIN32` preprocessor macro is defined
   - Uses WinDivert for packet capture and manipulation

**Design Contract:** All implementations inherit from `SizingMainControllerPrototype` and implement the interface method:

```cpp
virtual Commons::POSIXErrors start(int argC, char** argV) = 0;
```

Platform determination occurs at compile-time through preprocessor conditional compilation directives (`__linux__`, `_WIN32`, `OS_ID`).

### C. Component Organization

**Primary Directories:**

- **`Apps/`** — Application entry points
  - `Sizing/SizingMain.cpp`: Main executable entry point

- **`Sources/SizingControllers/`** — Platform-specific controller implementations
  - `SizingMainController.cpp`: Factory implementation
  - `LinuxSizingMainController.cpp`: Linux generic controller
  - `LinuxCentosSizingMainController.cpp`: Linux CentOS controller
  - `WindowsSizingMainController.cpp`: Windows controller
  - `SizingMainControllerPrototype.cpp`: Abstract base class

- **`Services/SizingServices/`** — Business logic and data transformation services
  - `Transformer.cpp`: Data transformation utilities

- **`Models/`** — Data models and utility components
  - `Commons/`: Core utilities (HashTable, Log, POSIXErrors, StringImplement, Time, IOExporters)
  - `FileParsers/`: Configuration file parsers (InitializedJsonFileParser, etc.)
  - `PCAP/`: Packet capture abstractions (LinuxPCAP, WindowsPCAP, PCAPPrototype)

- **`Headers/`** — Public interface declarations (mirrors source structure)

- **`Settings/`** — Configuration management
  - `.Cmake/`: Modular CMake configuration files
  - `.Json/`: Runtime configuration (SizingMain.json, globalDependencies.json)
  - `.version`: Version identifier

- **`Shells/`** — Build automation scripts
  - `iniParser.sh`: INI file parser
  - `codecUtilities.sh`: Code utilities
  - `installVendor.sh`: Dependency installation
  - `makeVersion.sh`: Version management
  - `compiledStringMessage.sh`: Compile-time string generation

- **`Tests/`** — Unit test suite (mirrors source structure)

- **`Vendors/`** — External dependencies (libpcap, cJson, WinDivert)

- **`Bin/`** — Compiled executables

- **`Outputs/`** — Runtime output files (created during execution)

- **`Logs/`** — Application log files

### D. CMake Configuration System

The build system employs modular CMake configuration files located in `Settings/.Cmake/`:

1. **`.globalCmakeVariable.cmake`** — Global variable documentation
2. **`.systemSupport.cmake`** — Operating system detection and `OPERATING_SYSTEM` variable assignment
3. **`.testingInstruction.cmake`** — `BUILD_TEST` flag control (default: ON)
4. **`.customFileReference.cmake`** — Binary directory file specifications
5. **`.projectDirectoryIncluded.cmake`** — Project directory inclusion list
6. **`.compiledRouter.cmake`** — Conditional compilation routing:
   - `BUILD_TEST=ON`: Routes to `Tests/` subdirectory
   - `BUILD_TEST=OFF`: Routes to `Apps/` subdirectory

### E. Dependency Management

External dependencies are managed through `Settings/.Json/globalDependencies.json`:

1. **libpcap** (Linux only)
   - Source: https://www.tcpdump.org/release/
   - Version: 1.9.1
   - Purpose: Network packet capture library

2. **cJson** (Cross-platform)
   - Source: https://github.com/DaveGamble/cJSON
   - Purpose: JSON parsing and generation

3. **WinDivert** (Windows only)
   - Source: https://github.com/basil00/WinDivert
   - Version: 2.2.2-A
   - Purpose: User-mode packet capture and manipulation

Dependencies are automatically resolved and built by `processMake.sh` through `Shells/installVendor.sh`.

---

## IV. CONFIGURATION

### A. Runtime Configuration

The application configuration is specified in `Settings/.Json/SizingMain.json`:

```json
{
  "base": {
    "service": [
      {
        "interface": "ens192",
        "port": ["80"]
      }
    ],
    "writingFileSecond": 10,
    "outputLayoutType": 1
  }
}
```

**Configuration Parameters:**
- `interface`: Network interface identifier for packet capture
- `port`: Array of TCP/UDP ports to monitor
- `writingFileSecond`: Output file write interval (seconds)
- `outputLayoutType`: Output format specification (integer enumeration)

### B. Platform-Specific Considerations

**Windows Platform:**
- Administrator privileges required for execution (WinDivert driver installation)
- Administrator privileges required for debugging sessions
- WinDivert runtime files (`WinDivert.dll`, `WinDivert64.sys`) automatically deployed to `Bin/` directory

**Linux Platform:**
- Standard user privileges sufficient
- Root privileges may be required for promiscuous mode packet capture on certain interfaces

---

## V. DEVELOPMENT CONVENTIONS

### A. Namespace Organization

The codebase employs namespace-based component organization:

**Sizing Application:**
- `SizingControllers::` — Controller implementations
- `SizingServices::` — Service layer components

**PacketRedirection Application:**
- `PacketRedirectionControllers::` — Controller implementations
- `PacketRedirectionServices::` — Load balancing and statistics services

**Shared Components:**
- `Commons::` — Common utility functions and classes
- `FileParsers::` — Configuration file parsing utilities
- `PCAP::` — Packet capture interface abstractions

### B. Error Handling Strategy

The system implements standardized error reporting through the `Commons::POSIXErrors` enumeration, providing consistent error codes across platform-specific implementations.

### C. Code Style and Naming Conventions

**Recent Architectural Changes (October 2024):**
- Terminology change: "Caller" → "Controller"
- Example: `SizingMainCaller` renamed to `SizingMainController`
- Rationale: Improved semantic clarity and alignment with design pattern nomenclature

---

## VI. EXTENDING PLATFORM SUPPORT

To add support for additional platforms or operating system distributions:

**Step 1:** Create a new controller class inheriting from `SizingMainControllerPrototype`
```cpp
class NewPlatformSizingMainController : public SizingMainControllerPrototype {
public:
    virtual Commons::POSIXErrors start(int argC, char** argV) override;
};
```

**Step 2:** Implement the required `start()` method with platform-specific packet capture logic

**Step 3:** Add platform detection logic in `Sources/SizingControllers/SizingMainController.cpp`:
```cpp
#ifdef NEW_PLATFORM_MACRO
    NewPlatformSizingMainController instance;
#endif
```

**Step 4:** Update CMake configuration files to define appropriate preprocessor macros for the new platform

**Step 5:** Update dependency configuration in `Settings/.Json/globalDependencies.json` if platform-specific libraries are required

---

## VII. PACKETREDIRECTION APPLICATION

### A. Overview

The PacketRedirection application implements a network packet forwarding system with load balancing capabilities. It captures network packets from specified interfaces and redistributes them across multiple target hosts using a round-robin algorithm.

**Key Features:**
- Multi-target packet forwarding
- Round-robin load balancing
- Real-time statistics tracking
- Cross-platform architecture (Linux primary, Windows framework available)

### B. Configuration

The application configuration is specified in `Settings/.Json/PacketRedirectionMain.json`:

```json
{
  "base": {
    "capture": {
      "interface": "ens192",
      "port": ["80", "443"]
    },
    "targets": [
      {"host": "192.168.1.100", "port": 8080},
      {"host": "192.168.1.101", "port": 8080},
      {"host": "192.168.1.102", "port": 8080}
    ],
    "loadBalancer": {
      "algorithm": "round-robin"
    },
    "statistics": {
      "enabled": true,
      "writingFileSecond": 10,
      "outputLayoutType": 1
    }
  }
}
```

**Configuration Parameters:**
- `capture.interface`: Network interface identifier for packet capture
- `capture.port`: Array of TCP/UDP ports to monitor
- `targets`: Array of redirection targets with host and port
- `loadBalancer.algorithm`: Load balancing algorithm (currently "round-robin")
- `statistics.writingFileSecond`: Statistics output interval (seconds)
- `statistics.outputLayoutType`: Output format specification (0=text, 1=TSV)

### C. Execution

**Build the application:**
```bash
./processMake.sh
```

**Run the application (requires root privileges for raw socket):**
```bash
sudo ./Bin/PacketRedirectionMain
```

**Output files:**
- Statistics are written to `Outputs/packetRedirection_<timestamp>.tsv`
- Format includes: Target Host, Target Port, Sent Packets, Sent Bytes, Failed Packets

### D. Architecture Details

**Controller Pattern:**
The application uses the same factory pattern as Sizing:
- `PacketRedirectionMainController` (factory)
- `LinuxPacketRedirectionMainController` (Linux implementation)
- `LinuxCentosPacketRedirectionMainController` (CentOS variant)
- `WindowsPacketRedirectionMainController` (framework for future implementation)

**Service Components:**
1. **LoadBalancer**: Implements round-robin target selection
   - `selectTarget()`: Returns next target in rotation
   - `setTargets()`: Configure target list

2. **RedirectionStatistics**: Thread-safe statistics tracking
   - `recordSent()`: Record successful packet transmission
   - `recordFailure()`: Record failed transmission
   - `exportToFile()`: Write statistics to file

**Packet Flow:**
1. Capture packets using `PCAP::LinuxPCAP` on specified interface/ports
2. Select target using `LoadBalancer::selectTarget()`
3. Forward packet using raw socket (`SOCK_RAW`) to selected target
4. Record statistics via `RedirectionStatistics::recordSent()` or `recordFailure()`
5. Periodically export statistics to file (triggered by SIGALRM)

### E. Platform-Specific Requirements

**Linux (RHEL 8.7):**
- Root privileges required for raw socket creation
- Uses raw sockets (`AF_INET`, `SOCK_RAW`, `IPPROTO_RAW`)
- Automatic setuid configuration via CMake (`chmod 4755`)

**Windows (Future):**
- Framework provided but implementation pending
- Will use WinDivert for packet injection
- Administrator privileges will be required

### F. Extending the Application

**Adding New Load Balancing Algorithms:**

1. Modify `Services/PacketRedirectionServices/Headers/LoadBalancer.hpp`:
```cpp
enum class LoadBalancingAlgorithm {
    ROUND_ROBIN,
    LEAST_CONNECTIONS,  // New algorithm
    WEIGHTED_ROUND_ROBIN
};
```

2. Implement algorithm in `LoadBalancer.cpp`:
```cpp
RedirectionTarget* LoadBalancer::selectTargetLeastConnections() {
    // Implementation
}
```

3. Update configuration parser to handle new algorithm type

---

## VIII. REFERENCES

[1] The Tcpdump Group, "libpcap," https://www.tcpdump.org/

[2] D. Gamble, "cJSON," https://github.com/DaveGamble/cJSON

[3] B. Hubert, "WinDivert: Windows Packet Divert," https://github.com/basil00/WinDivert

[4] IEEE, "IEEE Standard for Software Design Descriptions," IEEE Std 1016-2009

[5] Red Hat, Inc., "Red Hat Enterprise Linux 8.7 Release Notes"
