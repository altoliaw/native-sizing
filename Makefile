# Project root path from (to project)
Prdir:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
Headers=${Prdir}/Headers
Sources=${Prdir}/Sources

# Setting of the general compiled grammer
## The Compiler, library, level of the compiler optimization, detected information, wall and message
Cmp=g++
#Stdlib=-std=c99
Stdlib=-std=c++98
Cmpopt=-O0
Detinfo=-g0
Wall=-Wall
Fsg=-fmessage-length=0 -pthread
CFlags=
LDFlags=
ThirdLibDefinition=

## Project execution's name
PjN:= $(word $(words $(subst /, ,${Prdir})), $(subst /, ,${Prdir}))

# Make's Processes
.Suffixs: .c .h .cpp .hpp

.Phony: all
all: ${Prdir}/${PjN}
	@make run

.Phony: build
build: ${Prdir}/${PjN}/build

.Phony: clean
clean:
	@clear
	@rm -rf ${Sources}/*.o
	@rm -rf ${Sources}/*/*.o
	@rm -rf ${Prdir}/${PjN}
	@rm -rf ${Prdir}/*.o

.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${Prdir}/build/*

.Phony: run
run:
	${Prdir}/${PjN}

##================================================================
# Build libraries
${Prdir}/${PjN}/build : 
			

# Create a application
${Prdir}/${PjN}: 	${Prdir}/Main.o ${Sources}/MainCaller.o

	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Fsg} -o ${Prdir}/${PjN} ${Prdir}/Main.o \
	${Sources}/MainCaller.o \
	${ThirdLibDefinition}

# Main
${Prdir}/Main.o:	${Headers}/MainCaller.hpp ${Prdir}/Main.cpp

	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Prdir}/Main.cpp -c ${Fsg} -o ${Prdir}/Main.o

# MainCaller
${Sources}/MainCaller.o:	${Headers}/MainCaller.hpp ${Sources}/MainCaller.cpp

	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Sources}/MainCaller.cpp -c ${Fsg} -o ${Sources}/MainCaller.o
