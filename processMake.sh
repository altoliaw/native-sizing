#!/bin/bash

# Considering the dependency of the native sizing by the traversal of the structure in the file, globalDependencies.json
Vendors="Vendors"
source $(pwd)/Shells/installVendor.sh && \
dependenciesTraversal $(pwd)/Settings/.Json/globalDependencies.json $(pwd)/$Vendors/.$Vendors.json


# Parsing the parameters into the array; these are defined as below:
# --remove | --r | -r
# The array mentioned previously, is for triggering the parameter's processing.
ParameterArray=("$@") # Putting all parameters into the array
for i in "${!ParameterArray[@]}"; do  # ${#ParameterArray[@]} implies the number of element indexes
	Param="${ParameterArray[$i]}"
	case "$Param" in
		--remove|--r|-r)
			rm -rf build
			echo -e "Build directory has been removed."
	esac
done

# Folders' generations
mkdir -p Outputs
mkdir -p Logs
mkdir -p Bin		# Creating the folder for execution

# Cmake process
OsType="$(uname -s | tr '[:upper:]' '[:lower:]')" # Obtaining the kernel type string and then translating the string with the lower case

if [[ "$OsType" = "linux" ]]; then
    echo "Linux build process..."
    mkdir -p build
    cmake -S . -B build -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    cp -f build/Apps/Sizing/SizingMain ./Bin/
    echo "SizingMain copied to Bin directory."
    echo "No any pre-processes are necessary."

elif [[ "$OsType" = *"mingw"* ]]; then
    echo "Windows dual-kernel build process..."
    mkdir -p build

    # --- NPCAP Build ---
    echo "Building NPCAP version..."
    cmake -S . -B build -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug -DWINDOWS_PCAP_KERNEL=NPCAP
    cmake --build build
    cp -f build/Apps/Sizing/SizingMain.exe ./Bin/SizingMain_npcap.exe
    echo "SizingMain_npcap.exe created in Bin directory."

    # --- Clean ---
    echo "Cleaning build directory for next build..."
    cmake --build build --target clean

    # --- WinDivert Build ---
    echo "Building WinDivert version..."
    cmake -S . -B build -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug -DWINDOWS_PCAP_KERNEL=WINDIVERT
    cmake --build build
    cp -f build/Apps/Sizing/SizingMain.exe ./Bin/SizingMain_windivert.exe
    echo "SizingMain_windivert.exe created in Bin directory."

    # --- Post-processing for WinDivert ---
    echo "Performing post-processing for WinDivert..."
    sc stop WinDivert >/dev/null 2>&1
    sc delete WinDivert >/dev/null 2>&1
    cp -f ./Vendors/WinDivert/Libs/WinDivert.dll ./Bin/
    cp -f ./Vendors/WinDivert/Libs/WinDivert64.sys ./Bin/
    echo -e "The post-processing on the Windows has been executed. Executables are already in Bin."
else
    echo "Unsupported OS: $OsType"
    # Fallback to default build for other OS types if necessary
    mkdir -p build
    cmake -S . -B build -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
fi
exit

# cd build
# cp -ar ./Apps/SizingMain_Windows.exe ../Bin/
# cd ..