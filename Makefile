# The main makefile to compile all apps
# Obtaining the project root path (to project's path)
Prdir:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# Setting of the general compiled grammar
## The Compiler, library, level of the compiler optimization, detected information, WALL and message
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
	@sudo chown root:root ${Prdir}/${Bin}/${PjN}_Sizing
	@sudo chmod 4755 ${Prdir}//${Bin}/${PjN}_Sizing
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
	@sudo rm -rf ${Prdir}/${Bin}/${PjN}_Sizing

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
	@sudo chown root:root ${Prdir}/${Bin}/${PjN}_Sizing
	@sudo chmod 4755 ${Prdir}/${Bin}/${PjN}_Sizing
			
# The location for creating folders & a maintained file in advance
${Prdir}/${PjN}/Folders:
	@mkdir -p ${Bin}  					# Creating the folder for execution

##===============[Application]=========================================
# Create an application
${Prdir}/${PjN}_Sizing: 	${Prdir}/${AppLoc}/SysinMain.o \
							${Sources}/SysinMainCallerPrototype.o \
							${Sources}/LinuxSysinMainCaller.o \
							${Sources}/SysinMainCaller.o \
							${Commons.Sources}/HashTable.o \
							${Commons.Sources}/IOSpecification.o \
							${Commons.Sources}/StringImplement.o \
							${Commons.Sources}/Time.o \
							${FileParsers.Sources}/InitializedJsonFileParser.o \
							${PCAP.Sources}/PCAPPrototype.o \
							${PCAP.Sources}/LinuxPCAP.o

	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${FMSG} -o ${Prdir}/${Bin}/${PjN}_Sizing ${Prdir}/${AppLoc}/SysinMain.o \
	${Sources}/SysinMainCallerPrototype.o \
	${Sources}/LinuxSysinMainCaller.o \
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
${Prdir}/${AppLoc}/SysinMain.o:	${Headers}/SysinMainCaller.hpp ${Prdir}/${AppLoc}/SysinMain.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Prdir}/${AppLoc}/SysinMain.cpp -c ${FMSG} -o ${Prdir}/${AppLoc}/SysinMain.o

# SysinMainCaller
${Sources}/SysinMainCaller.o:	${Headers}/SysinMainCaller.hpp \
								${Sources}/SysinMainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Sources}/SysinMainCaller.cpp -c ${FMSG} -o ${Sources}/SysinMainCaller.o

# SysinMainCallerPrototype
${Sources}/SysinMainCallerPrototype.o:	${Headers}/SysinMainCallerPrototype.hpp ${Sources}/SysinMainCallerPrototype.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Sources}/SysinMainCallerPrototype.cpp -c ${FMSG} -o ${Sources}/SysinMainCallerPrototype.o

# LinuxSysinMainCaller
${Sources}/LinuxSysinMainCaller.o:	${Commons.Headers}/POSIXErrors.hpp \
								${Commons.Headers}/IOSpecification.hpp \
								${Commons.Headers}/Time.hpp \
								${FileParsers.Headers}/InitializedJsonFileParser.hpp \
								${PCAP.Headers}/LinuxPCAP.hpp \
								${Headers}/LinuxSysinMainCaller.hpp \
								${Sources}/LinuxSysinMainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Sources}/LinuxSysinMainCaller.cpp -c ${FMSG} -o ${Sources}/LinuxSysinMainCaller.o


##===============[Dependencies]=========================================
# Commons.HashTable
${Commons.Sources}/HashTable.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/HashTable.hpp ${Commons.Sources}/HashTable.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/HashTable.cpp -c ${FMSG} -o ${Commons.Sources}/HashTable.o

# Commons.IOSpecification
${Commons.Sources}/IOSpecification.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/IOSpecification.hpp ${Commons.Sources}/IOSpecification.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/IOSpecification.cpp -c ${FMSG} -o ${Commons.Sources}/IOSpecification.o

# Commons.StringImplement
${Commons.Sources}/StringImplement.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/StringImplement.hpp ${Commons.Sources}/StringImplement.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/StringImplement.cpp -c ${FMSG} -o ${Commons.Sources}/StringImplement.o

# Commons.Time
${Commons.Sources}/Time.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/Time.hpp ${Commons.Sources}/Time.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/Time.cpp -c ${FMSG} -o ${Commons.Sources}/Time.o

# FileParsers.InitializedFileParser
${FileParsers.Sources}/InitializedFileParser.o:	${Commons.Headers}/HashTable.hpp ${FileParsers.Headers}/InitializedFileParser.hpp ${FileParsers.Sources}/InitializedFileParser.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${FileParsers.Sources}/InitializedFileParser.cpp -c ${FMSG} -o ${FileParsers.Sources}/InitializedFileParser.o

# FileParsers.InitializedJsonFileParser
${FileParsers.Sources}/InitializedJsonFileParser.o:	${Vendors.Headers}/cJSON.h ${FileParsers.Headers}/InitializedJsonFileParser.hpp ${FileParsers.Sources}/InitializedJsonFileParser.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${FileParsers.Sources}/InitializedJsonFileParser.cpp -c ${FMSG} -o ${FileParsers.Sources}/InitializedJsonFileParser.o

# PCAP.PCAPPrototype
${PCAP.Sources}/PCAPPrototype.o:	${PCAP.Headers}/PCAPPrototype.hpp ${PCAP.Sources}/PCAPPrototype.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${PCAP.Sources}/PCAPPrototype.cpp -c ${FMSG} -o ${PCAP.Sources}/PCAPPrototype.o

# PCAP.LinuxPCAP
${PCAP.Sources}/LinuxPCAP.o:	${PCAP.Headers}/PCAPPrototype.hpp ${PCAP.Headers}/LinuxPCAP.hpp ${PCAP.Sources}/LinuxPCAP.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${PCAP.Sources}/LinuxPCAP.cpp -c ${FMSG} -o ${PCAP.Sources}/LinuxPCAP.o