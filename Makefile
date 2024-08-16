# The main makefile to compile all apps
# Operating system
OS:=$(shell uname -s)
SUDO=
ifeq (${OS}, Linux)
	SUDO=sudo
endif


# Obtaining the project root path (to project's path)
Prdir:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# Setting of the general compiled grammar
## The compiler, library, level of the compiler optimization, detected information, WALL and message
## Obtaining the all compiler setting string from globalCompiling.ini and the function, iniParser_getIni, in the file, iniParser.sh;
## the return value is "compiler.STD=-std=c++11;compiler.DETAILINFO=-g0; ..."
KVPAIR:=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(iniParser_getIni ${Prdir}/Settings/.Ini/globalCompiling.ini))
## Obtaining the compiling information
CC			=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.CC"))
STD			=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.STD"))
CMPOPT		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.CMPOPT"))
DETAILINFO	=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.DETAILINFO"))
WALL		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.WALL"))
FMSG		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.FMSG"))
CFLAGS		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.CFLAGS"))
LDFLAGS		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.LDFLAGS"))
LDLIBS		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "compiler.LDLIBS"))

ifneq ($(OS), Linux)
LDLIBS		=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "windows.LDLIBS"))
endif


# ------ 
# Variable definition
Headers=${Prdir}/Headers
Sources=${Prdir}/Sources

Commons.Headers=${Prdir}/Models/Commons/Headers
Commons.Sources=${Prdir}/Models/Commons/Sources

FileParsers.Headers=${Prdir}/Models/FileParsers/Headers
FileParsers.Sources=${Prdir}/Models/FileParsers/Sources

PCAP.Headers=${Prdir}/Models/PCAP/Headers
PCAP.Sources=${Prdir}/Models/PCAP/Sources

Vendors.Headers=${Prdir}/Vendors/Includes


## Project execution's name
PjN:= $(word $(words $(subst /, ,${Prdir})), $(subst /, ,${Prdir}))
## The folder name for the start entry point of the cpp of the execution
AppLoc:=Apps

## The necessary folders name for the project
### The folder for the exection
Bin:=Bin
### The foldeer for the third party software
Vendors:=Vendors

# Make's Processes
.Suffixs: .c .h .cpp .hpp

.Phony: all
all:	${Prdir}/${PjN}/Folders ${Prdir}/${PjN}_Sizing
	@mkdir -p Outputs
	@mkdir -p Logs
ifeq (${OS}, Linux)
		@${SUDO} chown root:root ${Prdir}/${Bin}/${PjN}_Sizing
endif
	@${SUDO} chmod 4755 ${Prdir}//${Bin}/${PjN}_Sizing
	@echo ""
	@echo "=================[Execution]===================="
	@echo ""
	@make run
	

.Phony: build
build: ${Prdir}/${PjN}/Folders ${Prdir}/${PjN}_Sizing/build

.Phony: clean
clean:
	@clear
	@rm -rf ${Prdir}/*.o
	@rm -rf ${Prdir}/*/*.o
	@rm -rf ${Prdir}/*/*/*.o
	@rm -rf ${Prdir}/*/*/*/*.o
	@${SUDO} rm -rf ${Prdir}/${Bin}/${PjN}_Sizing

.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${Prdir}/build

.Phony: cmake
cmake: ${Prdir}/${PjN}/Folders
	cmake -S . -B build
	cmake --build build
	cd build && ctest -V

.Phony: vendor
vendor: ${Prdir}/${PjN}/Folders
    # Creating the vendor and related folders by using the function, dependenciesTraversal, 
    # from the shell script, installVendor.sh, and determining the existence of the file & initializing the file
	@source ${Prdir}/Shells/installVendor.sh && dependenciesTraversal "${Prdir}/Settings/.Json/globalDependencies.json" "${Prdir}/${Vendors}/.${Vendors}.json"

# To add the version 
# .Phony: version
# version: ${Prdir}/${PjN}/Folders
# 	@make -f Apps/Console/Makefile_Process.mk "Prdir=${Prdir}"
# 	@make -f Apps/Console/Makefile_Process.mk clean


##----------------------------------------------------------------------
## Nick editted manually
## ---------------------------------------------------------------------
##dbsecure ALL=NOPASSWD: /bin/rm -rf /home/dbsecure/trafficMonitor/trafficMonitor
.Phony: run
run:	
	${Prdir}/${Bin}/${PjN}_Sizing

# Build
${Prdir}/${PjN}_Sizing/build : ${Prdir}/${PjN}_Sizing
ifeq (${OS}, Linux)
	@${SUDO} chown root:root ${Prdir}/${Bin}/${PjN}_Sizing
endif
	@${SUDO} chmod 4755 ${Prdir}/${Bin}/${PjN}_Sizing
			
# The location for creating folders & a maintained file in advance
${Prdir}/${PjN}/Folders:
	@mkdir -p ${Bin}  					# Creating the folder for execution

##===============[Application]=========================================
# Create an application
${Prdir}/${PjN}_Sizing: 	${Prdir}/${AppLoc}/SysinMain.o \
							${Sources}/SysinMainCallerPrototype.o \
							${Sources}/LinuxSysinMainCaller.o \
							${Sources}/WindowsSysinMainCaller.o \
							${Sources}/SysinMainCaller.o \
							${Commons.Sources}/HashTable.o \
							${Commons.Sources}/IOSpecification.o \
							${Commons.Sources}/StringImplement.o \
							${Commons.Sources}/Time.o \
							${FileParsers.Sources}/InitializedJsonFileParser.o \
							${PCAP.Sources}/PCAPPrototype.o \
							${PCAP.Sources}/LinuxPCAP.o

	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${FMSG} -o ${Prdir}/${Bin}/${PjN}_Sizing ${Prdir}/${AppLoc}/SysinMain.o \
	${Sources}/SysinMainCallerPrototype.o \
	${Sources}/LinuxSysinMainCaller.o \
	${Sources}/WindowsSysinMainCaller.o \
	${Sources}/SysinMainCaller.o \
	${Commons.Sources}/HashTable.o \
	${Commons.Sources}/IOSpecification.o \
	${Commons.Sources}/StringImplement.o \
	${Commons.Sources}/Time.o \
	${FileParsers.Sources}/InitializedJsonFileParser.o \
	${PCAP.Sources}/PCAPPrototype.o \
	${PCAP.Sources}/LinuxPCAP.o \
	${LDFLAGS} \
	${LDLIBS}

# SysinMain
${Prdir}/${AppLoc}/SysinMain.o:	${Prdir}/${AppLoc}/SysinMain.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Prdir}/${AppLoc}/SysinMain.cpp -c ${FMSG} -o ${Prdir}/${AppLoc}/SysinMain.o

# SysinMainCaller
${Sources}/SysinMainCaller.o:	${Sources}/SysinMainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Sources}/SysinMainCaller.cpp -c ${FMSG} -o ${Sources}/SysinMainCaller.o

# SysinMainCallerPrototype
${Sources}/SysinMainCallerPrototype.o:	${Sources}/SysinMainCallerPrototype.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Sources}/SysinMainCallerPrototype.cpp -c ${FMSG} -o ${Sources}/SysinMainCallerPrototype.o

# LinuxSysinMainCaller
${Sources}/LinuxSysinMainCaller.o:	${Sources}/LinuxSysinMainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Sources}/LinuxSysinMainCaller.cpp -c ${FMSG} -o ${Sources}/LinuxSysinMainCaller.o

# WindowsSysinMainCaller
${Sources}/WindowsSysinMainCaller.o:	${Sources}/WindowsSysinMainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Sources}/WindowsSysinMainCaller.cpp -c ${FMSG} -o ${Sources}/WindowsSysinMainCaller.o



##===============[Dependencies]=========================================
# Commons.HashTable
${Commons.Sources}/HashTable.o:	${Commons.Sources}/HashTable.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Commons.Sources}/HashTable.cpp -c ${FMSG} -o ${Commons.Sources}/HashTable.o

# Commons.IOSpecification
${Commons.Sources}/IOSpecification.o:	 ${Commons.Sources}/IOSpecification.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Commons.Sources}/IOSpecification.cpp -c ${FMSG} -o ${Commons.Sources}/IOSpecification.o

# Commons.StringImplement
${Commons.Sources}/StringImplement.o:	${Commons.Sources}/StringImplement.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Commons.Sources}/StringImplement.cpp -c ${FMSG} -o ${Commons.Sources}/StringImplement.o

# Commons.Time
${Commons.Sources}/Time.o:	${Commons.Sources}/Time.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Commons.Sources}/Time.cpp -c ${FMSG} -o ${Commons.Sources}/Time.o

# FileParsers.InitializedFileParser
${FileParsers.Sources}/InitializedFileParser.o:	${FileParsers.Sources}/InitializedFileParser.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${FileParsers.Sources}/InitializedFileParser.cpp -c ${FMSG} -o ${FileParsers.Sources}/InitializedFileParser.o

# FileParsers.InitializedJsonFileParser
${FileParsers.Sources}/InitializedJsonFileParser.o:	${FileParsers.Sources}/InitializedJsonFileParser.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${FileParsers.Sources}/InitializedJsonFileParser.cpp -c ${FMSG} -o ${FileParsers.Sources}/InitializedJsonFileParser.o

# PCAP.PCAPPrototype
${PCAP.Sources}/PCAPPrototype.o:	${PCAP.Sources}/PCAPPrototype.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${PCAP.Sources}/PCAPPrototype.cpp -c ${FMSG} -o ${PCAP.Sources}/PCAPPrototype.o

# PCAP.LinuxPCAP
${PCAP.Sources}/LinuxPCAP.o:	${PCAP.Sources}/LinuxPCAP.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${PCAP.Sources}/LinuxPCAP.cpp -c ${FMSG} -o ${PCAP.Sources}/LinuxPCAP.o