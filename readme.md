# Traffic Monitor, Sizing <p align ="left" style="font-size: 14px;">The issue was raised on May 15th, 2024, edited by Nick.</p>
<!--Introduction-->
To facilitate a rapid assessment of packet sizes across heterogeneous operating systems, we contribute a C++-based tool that identifies the classes of devices supported by the DBAEGIS decoders. The tool has been validated on both RHEL 8.7 and Windows 10 platforms. Details regarding the development environment and configuration settings are provided in the following sections.

## Development Environment
Here are the necessary tools for developing and compiling the project on RHEL8.7:

g++ (g++ (GCC) 8.5.0 20210514 (Red Hat 8.5.0-15)) \
gcc (GCC) 8.5.0 20210514 (Red Hat 8.5.0-15) \
GNU Make 4.2.1 \
cmake version 3.20.2 \
jq-1.6 \
GNU Awk 4.2.1
git


## Running Phase
The current working directory is the project root. Users can use make to compile the codes
and run the execution, users can use the command `make`. If someone do not run the codes, please use
the command, `make build`.
