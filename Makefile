# Obtaining the project root path (to project's path)
Prdir:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# Setting of the general compiled grammer
## The Compiler, library, level of the compiler optimization, detected information, WALL and message
## Obtaining the all compiler setting string from globalCompiling.ini and the function, iniParser_getIni, in the file, iniParser.sh;
## the return value is "compiler.STD=-std=c++11;compiler.DETAILINFO=-g0; ..."
KVPAIR:=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(iniParser_getIni ${Prdir}/.Ini/globalCompiling.ini))
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
Headers=${Prdir}/Headers
Sources=${Prdir}/Sources

Commons.Headers=${Prdir}/Models/Commons/Headers
Commons.Sources=${Prdir}/Models/Commons/Sources

PCAP.Headers=${Prdir}/Models/PCAP/Headers
PCAP.Sources=${Prdir}/Models/PCAP/Sources


## Project execution's name
PjN:= $(word $(words $(subst /, ,${Prdir})), $(subst /, ,${Prdir}))

# Make's Processes
.Suffixs: .c .h .cpp .hpp

.Phony: all
all:	${Prdir}/${PjN}
	@sudo chown root:root ${Prdir}/${PjN}
	@sudo chmod 4755 ${Prdir}/${PjN}
	@echo ""
	@echo "=================[Execution]===================="
	@make run
	

.Phony: build
build: ${Prdir}/${PjN}/build

.Phony: clean
clean:
	@clear
	@rm -rf ${Prdir}/*.o
	@rm -rf ${Prdir}/*/*.o
	@rm -rf ${Prdir}/*/*/*.o
	@rm -rf ${Prdir}/*/*/*/*.o
	@sudo rm -rf ${Prdir}/${PjN}

.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${Prdir}/build/*


##----------------------------------------------------------------------
## Nick editted manually
## ---------------------------------------------------------------------
##dbsecure ALL=NOPASSWD: /bin/rm -rf /home/dbsecure/trafficMonitor/trafficMonitor
.Phony: run
run:	
	${Prdir}/${PjN}

# Build libraries
${Prdir}/${PjN}/build : 
			

##================================================================
# Create a application
${Prdir}/${PjN}: 	${Prdir}/Main.o \
					${Sources}/MainCaller.o \
					${Commons.Sources}/HashTable.o \
					${Commons.Sources}/IOSpecification.o \
					${PCAP.Sources}/LinuxPCAP.o

	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${FMSG} -o ${Prdir}/${PjN} ${Prdir}/Main.o \
	${Sources}/MainCaller.o \
	${Commons.Sources}/HashTable.o \
	${Commons.Sources}/IOSpecification.o \
	${PCAP.Sources}/LinuxPCAP.o \
	${LDLIBS}

# Main
${Prdir}/Main.o:	${Headers}/MainCaller.hpp ${Prdir}/Main.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Prdir}/Main.cpp -c ${FMSG} -o ${Prdir}/Main.o

# MainCaller
${Sources}/MainCaller.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/IOSpecification.hpp ${Headers}/MainCaller.hpp ${Sources}/MainCaller.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Sources}/MainCaller.cpp -c ${FMSG} -o ${Sources}/MainCaller.o

# Commons.HashTable
${Commons.Sources}/HashTable.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/HashTable.hpp ${Commons.Sources}/HashTable.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/HashTable.cpp -c ${FMSG} -o ${Commons.Sources}/HashTable.o

# Commons.IOSpecification
${Commons.Sources}/IOSpecification.o:	${Commons.Headers}/POSIXErrors.hpp ${Commons.Headers}/IOSpecification.hpp ${Commons.Sources}/IOSpecification.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${Commons.Sources}/IOSpecification.cpp -c ${FMSG} -o ${Commons.Sources}/IOSpecification.o

# PCAP.LinuxPCAP
${PCAP.Sources}/LinuxPCAP.o:	${PCAP.Headers}/PCAPPrototype.hpp ${PCAP.Headers}/LinuxPCAP.hpp ${PCAP.Sources}/LinuxPCAP.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${PCAP.Sources}/LinuxPCAP.cpp -c ${FMSG} -o ${PCAP.Sources}/LinuxPCAP.o