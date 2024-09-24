# The Makefile for Commons
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
Models			:=	Models.Commons
Duration		:=
Commons.Headers	:=	${projectDir}/Models/Commons/Headers
Commons.Sources	:=	${projectDir}/Models/Commons/Sources

# ######## [Implicit Rules]
# The definition for the basic element in the Makefile
.Suffixs: .c .h .cpp .hpp


# ######## [Phonies]
# The top level of the Makefile
.Phony: all
all: \
	startTime \
	${Commons.Sources}/HashTable.o \
	${Commons.Sources}/IOSpecification.o \
	${Commons.Sources}/StringImplement.o \
	${Commons.Sources}/Time.o \
	endTime


# ######## [Compiling Items]
# Start time
startTime:
	@$(eval Duration := $(shell source ${projectDir}/Shells/compiledStringMessage.sh && echo $$(startingString "${Models}")))
# End Time
endTime:
	@echo $(shell source ${projectDir}/Shells/compiledStringMessage.sh && echo $$(endingString "${Models}" "${Duration}"))

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