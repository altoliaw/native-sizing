# The main compilation for all application executions
#
# @author Nick
# @date 2024/09/20

# ######## [Environment Setting]
# Operating system
OS:= $(shell uname -s)
SUDO=
ifeq (${OS}, Linux)
	SUDO:=sudo
endif


# ######## [Project Directory Setting]
# Obtaining the project root path (to project's path); using this approach to obtain the path even though the current working directory
# does not in the place where the makefile locates
projectDir:= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))


# ######## [Setting for the Compiler]
# ######## The compiler, library, level of the compiler optimization, detected information, WALL and message
# Obtaining the all compiler setting string from globalCompiling.ini and the function, iniParser_getIni, in the file, iniParser.sh;
# the return value is "compiler.STD=-std=c++11;compiler.DETAILINFO=-g0; ..."
keyValuePair:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(iniParser_getIni ${projectDir}/Settings/.Ini/globalCompiling.ini))

# Obtaining the compiling information
CC			:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.CC"))
STD			:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.STD"))
CMPOPT		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.CMPOPT"))
DETAILINFO	:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.DETAILINFO"))
WALL		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.WALL"))
FMSG		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.FMSG"))
CFLAGS		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.CFLAGS"))
LDFLAGS		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.LDFLAGS"))
LDLIBS		:= $(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "compiler.LDLIBS"))

# When the platform is not equal to the Linux
ifneq ($(OS), Linux)
LDLIBS		:=$(shell source ${projectDir}/Shells/iniParser.sh && echo $$(getVariableValue "${keyValuePair}" "windows.LDLIBS"))
endif


# ######## [Variable Definition]
# The folder for the exection
Bin:= Bin
# The foldeer for the third party software
Vendors:= Vendors


# ######## [Implicit Rules]
# The definition for the basic element in the Makefile
.Suffixs: .c .h .cpp .hpp


# ######## [Phonies]
# The top level of the Makefile
.Phony: all
all:
	
# For removing all object files from the compiled files & all executions
.Phony: clean
clean:
	cd ${projectDir}
	@clear
	@echo "Removing all object files from the compiled files & all executions"
	find . -name "*.o" -type f -delete
	@${SUDO} rm -rf ${projectDir}/${Bin}


# ######## [Custom Defined Phonies]

#
.Phony: vendor
vendor: ${projectDir}/Folders
    # Creating the vendor and related folders by using the function, dependenciesTraversal, 
    # from the shell script, installVendor.sh, and determining the existence of the file & initializing the file
	@source ${projectDir}/Shells/installVendor.sh && dependenciesTraversal "${projectDir}/Settings/.Json/globalDependencies.json" "${projectDir}/${Vendors}/.${Vendors}.json"

.Phony: pwd
pwd:
	echo "${projectDir}"

.Phony: build
build: ${projectDir}/Folders ${projectDir}/build

.Phony: build_windows
build_windows: ${projectDir}/Folders
	@eval ./windowsMake.sh

.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${projectDir}/build
	@${SUDO} rm -rf ${projectDir}/${Bin}

.Phony: cmake
cmake: ${projectDir}/Folders
	cmake -S . -B build
	cmake --build build
	cd build && ctest -V


# ######## [Recipes]
# For building the applications
${projectDir}/build:

# The location for creating folders & a maintained file in advance
${projectDir}/Folders:
	@mkdir -p Outputs
	@mkdir -p Logs
	@mkdir -p ${Bin}  					# Creating the folder for execution		