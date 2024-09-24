# The Makefile for the execution from SizingMain.cpp
#
# @author Nick
# @date 2024/09/24

# ######## [Environment Setting]
# The variables will be obtain the Makefile from the project; using the "searchElement" with jq to search the contents
# from the Shells/installVendor.sh
PLAIN_ARGUMENTS	:=  $(shell source ${projectDir}/Shells/codecUtilities.sh && echo $$(acsiiHexToString "${ARGUMENTS}"))

CC			:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.CC' '-r'))
STD			:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.STD' '-r'))
CMPOPT		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.CMPOPT' '-r'))
DETAILINFO	:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.DETAILINFO' '-r'))
WALL		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.WALL' '-r'))
FMSG		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.FMSG' '-r'))
CFLAGS		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.CFLAGS' '-r'))
LDFLAGS		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.LDFLAGS' '-r'))
LDLIBS		:=	$(shell source ${projectDir}/Shells/installVendor.sh && echo $$(searchElement "${PLAIN_ARGUMENTS}" '.LDLIBS' '-r'))

# If someone feel like using the definitions that users define, please remove the comment and fill in the suitable terms.
# CC			=
# STD			=
# CMPOPT		=
# DETAILINFO	=
# WALL		=
# FMSG		=
# CFLAGS		=
# LDFLAGS		=
# LDLIBS		=


# ######## [Variable Definition]
Models			:=	Apps
Duration		:=

Apps			:=${projectDir}/Apps


# ######## [Implicit Rules]
# The definition for the basic element in the Makefile
.Suffixs: .c .h .cpp .hpp


# ######## [Phonies]
# The top level of the Makefile
.Phony: all
all: \
	startTime \
	${Apps}/SizingMain.o \
	endTime


# ######## [Compiling Items]
# Start time
startTime:
	@$(eval Duration := $(shell source ${projectDir}/Shells/compiledStringMessage.sh && echo $$(startingString "${Models}")))
# End Time
endTime:
	@echo $(shell source ${projectDir}/Shells/compiledStringMessage.sh && echo $$(endingString "${Models}" "${Duration}"))

# SizingMain
${Apps}/SizingMain.o:	${Apps}/SizingMain.cpp
	${CC} ${STD} ${CMPOPT} ${DETAILINFO} ${WALL} ${CFLAGS} ${Apps}/SizingMain.cpp -c ${FMSG} -o ${Apps}/SizingMain.o