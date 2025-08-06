# Traffic Monitor, Sizing 
<p align ="left" style="font-size: 14px;">The issue was raised on May 15th, 2024, edited by Nick and refactored by glu on June, 2025.</p>

<!--Introduction-->
To facilitate a rapid assessment of packet sizes across heterogeneous operating systems, we contribute a C++-based tool that identifies the classes of devices supported by the DBAEGIS decoders. The tool has been validated on both RHEL 8.7 and Windows 10 platforms. Details regarding the development environment and configuration settings are provided in the following sections.

## Development Environment
Here are the necessary tools for developing and compiling the project on RHEL8.7 / Msys2 (Windows platforms):

g++ (g++ (GCC) 8.5.0 20210514 (Red Hat 8.5.0-15)) \
gcc (GCC) 8.5.0 20210514 (Red Hat 8.5.0-15) \
GNU Make 4.2.1 \
cmake version 3.20.2 \
jq-1.6 \
unzip \
GNU Awk 4.2.1 \
git

## Version
Linux: libpcap version (RHEL 8.7) \
Windows: WinDivert version (Msys2, Mingw64) \
~~Windows: npcap version (Msys2, Mingw64)*~~
>
> *: Npcap version <= v0.0.2
## Running Phase
The current working directory is the project root. Users can use make to compile the codes with `./processMake.sh`. 


## Changes
- WindowsPCAP now uses WinDivert instead of NPCAP.
- Added `txGroupCount`, `rxGroupCount` and `flowChangeCount`, sizing now counts packet groups instead of individual packets.
- `WindowsSizingMainCaller::signalInterruptedHandler()` now uses `CancelIOEx()` to stop loop instead of global flag `_IS_PCAP_WORKED_`.

## Running tips
- Configure `Settings/Json/SizingMain.json`, replace ports to those the user wishes to intercept.
- Outputs are located in `Outputs`, which is created after execution.

## Others
- Linux platform
  - Not added `txGroupCount`, `rxGroupCount` and `flowChangeCount`,
- Windows platform
  - WinDivert files required for execution are copied from `Vendors` (this had been denoted in `processMake.sh`)
  - On windows platforms, users shall execute the execution with `administrator privilege` (right click).
  - Like the mentioned above, when users debug, the environment shall execute with `administrator privilege`.