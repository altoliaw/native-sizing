#!/bin/bash

# Considering the dependency of the native sizing by the traversal of the structure in the file, globalDependencies.json
Vendors="Vendors"

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
			;;
		--vendor|--v|-v)
			rm -rf $Vendors
			;;
	esac
done

# Considering the dependency of the native sizing by the traversal of the structure in the file, globalDependencies.json
source $(pwd)/Shells/installVendor.sh && \
dependenciesTraversal $(pwd)/Settings/.Json/globalDependencies.json $(pwd)/$Vendors/.$Vendors.json

# Folders' generations
mkdir -p Outputs
mkdir -p Logs
mkdir -p Bin		# Creating the folder for execution

# Cmake process
mkdir -p build
cmake -S . -B build -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build

OsType="$(uname -s | tr '[:upper:]' '[:lower:]')" # Obtaining the kernel type string and then translating the string with the lower case
# After the parameter process above, the pre-processing will come.
if [[ "$OsType" = "linux" ]]; then
	echo "No any pre-processes are necessary."
elif [[ "$OsType" = *"mingw"* ]]; then
	# Removing WinDivert service for avoiding that the service may exist already
	# By using the window service command, sc, the execution will stop and delete the WinDivert.
	# sc stop WinDivert | Out-Null
	# sc delete WinDivert | Out-Null
	sc stop WinDivert >/dev/null 2>&1
	sc delete WinDivert >/dev/null 2>&1
	# WinDivert.dll and WinDivert64.sys shall be stay with the executed file which refers to those WinDivert files. (TODO)
	cp -f ./Vendors/WinDivert/Libs/WinDivert.dll ./Bin/
	cp -f ./Vendors/WinDivert/Libs/WinDivert64.sys ./Bin/
	echo -e "The post-processing on the Windows has been executed."
fi
exit

# cd build
# cp -ar ./Apps/SizingMain_Windows.exe ../Bin/
# cd ..