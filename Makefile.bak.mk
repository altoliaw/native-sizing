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


# ######## [Variable Definitions]
# The folder for the exection
Bin:= Bin
# The foldeer for the third party software
Vendors:= Vendors
# The file for record the variables and values
TempMakefile:= ${projectDir}/tmp.mk

# ######## [Makefile Included]
include common.mk


# ######## [Makefile Arguments for sub-Makefile]
ARGUMENTS	:=	$(shell source ${projectDir}/Shells/codecUtilities.sh && echo $$(stringToAsciiHex '${arguments}'))

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
	@rm -f ${projectDir}/tmp.mk


# ######## [Custom Defined Phonies]
# The place for reserving the third libraries; the phony will install the libraries automatically as well
.Phony: vendor
vendor: ${projectDir}/Folders
    # Creating the vendor and related folders by using the function, dependenciesTraversal, 
    # from the shell script, installVendor.sh, and determining the existence of the file & initializing the file
	@source ${projectDir}/Shells/installVendor.sh && dependenciesTraversal "${projectDir}/Settings/.Json/globalDependencies.json" "${projectDir}/${Vendors}/.${Vendors}.json"

# Displaying the project of the working directory
.Phony: information
information:
	@echo "The project path is ${projectDir}"
	@echo "The common string modelled from all arguments is $(shell echo '${arguments}' | sed 's/\t//g')"	# Ignoring the tab
	@echo 'The hex values are ${ARGUMENTS}'

# Building all applications; depending on different platforms, the building approaches are different also
.Phony: build
build: ${projectDir}/Folders
# When the platform is equal to the Linux
ifeq ($(OS), Linux)
	@echo "" >${TempMakefile}
	@echo "[Linux Building]"
# @make -C Models/Commons all projectDir='${projectDir}' ARGUMENTS='${ARGUMENTS}' TempMakefile='${TempMakefile}'
# @make -C Models/FileParsers all projectDir='${projectDir}' ARGUMENTS='${ARGUMENTS}' TempMakefile='${TempMakefile}'
	@make -C Models/PCAP information projectDir='${projectDir}' ARGUMENTS='${ARGUMENTS}' TempMakefile='${TempMakefile}'
# @make -C Sources/SizingController all projectDir='${projectDir}' ARGUMENTS='${ARGUMENTS}' TempMakefile='${TempMakefile}'
# @make -C Apps all projectDir='${projectDir}' ARGUMENTS='${ARGUMENTS}' TempMakefile='${TempMakefile}'

else
	@echo "[Winndows Building]"
	@eval ./windowsMake.sh
endif
	
# The phony defines for removing files created by cmake. The created files can be generated by the "build phony" or "cmake phony".
.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${projectDir}/build
	@${SUDO} rm -rf ${projectDir}/${Bin}

# For unit tests
.Phony: cmake
cmake: ${projectDir}/Folders
	cmake -S . -B build
	cmake --build build
	cd build && ctest -V

# ######## [Recipes]
# The location for creating folders & a maintained file in advance
${projectDir}/Folders:
	@mkdir -p Outputs
	@mkdir -p Logs
	@mkdir -p ${Bin}		# Creating the folder for execution