
# Verifying the git application and leading the stderr (2) to /dev/null to 
# discard the error message 
GIT_INSTALLED:=$(shell command -v git 2> /dev/null)
GIT_HASH="None"
COPY_RIGHT_DATE=$(shell date +"%Y")

# If the git has been installed, the short committed hash value will be obtained.
ifdef GIT_INSTALLED
    # Obtaining git hash value
    GIT_HASH=$(shell git rev-parse --short HEAD)
endif

# Obtaining the key value pair from the .version file where ${Prdir} is passed by the caller makefile, Makefile
KVPAIR:=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(iniParser_getIni ${Prdir}/Settings/.version))
VERSION=$(shell source ${Prdir}/Shells/iniParser.sh && echo $$(getVariableValue "${KVPAIR}" "apps.version"))

.Phony: all
all:
	@echo ${Prdir}
	@echo ${VERSION}
	@echo ${KVPAIR}

.Phony: clean
clean: