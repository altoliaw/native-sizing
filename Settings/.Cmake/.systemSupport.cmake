# [System Support]
## This file sets the OPERATING_SYSTEM variable based on the platform. 
## If the platform does not match Linux or Windows, it will raise an error (exiting the process).
## To compile the windows version, users shall use MINGW64 from the MSYS2 environment. However, the platform is still detected as a Linux platform 
## in Cmake when compiling; as the result, the OS conditional shall be determine whether RHEL and MSYS2.
set(OPERATING_SYSTEM_STRING "Operating system flag (string format). 0 for Linux (RHEL), 0.1 for Linux (Centos), and 1 for Windows") # Description of the OPERATING_SYSTEM variable
if(UNIX)
    STRING(TOUPPER "${CMAKE_SYSTEM_NAME}" UPPER_CMAKE_SYSTEM_NAME) # Upper case CMAKE_SYSTEM_NAME for easier comparison
    ###########################################################################################################
    # Verifying if the system is Linux(RHEL series) or MSYS2 (which is a POSIX-compatible environment)
    if(UPPER_CMAKE_SYSTEM_NAME STREQUAL "LINUX") # RHEL/CentOS/... series

        # Due to different OS categories (RHEL and Centos), here the/etc/os-release file shall be used to the criterion for OS determination.
        if(EXISTS "/etc/os-release")
            # message(STATUS "CMAKE_SYSTEM_NAME2 = ${CMAKE_SYSTEM_NAME}")
            # Reading the file for determining the OS category
            file(READ "/etc/os-release" OS_RELEASE_CONTENTS)

            # Parsing the ID (RHEL or CentOS)
            string(REGEX MATCH "ID=\"?([a-zA-Z0-9]+)\"?" _match "${OS_RELEASE_CONTENTS}") # Using regex to match the ID line such as ID="rhel" or ID=centos
            STRING(TOUPPER "${CMAKE_MATCH_1}" OS_ID) # Extracting the matched ID and converting to upper case for easier comparison
            
            # Verifying the OS category in details
            if(OS_ID STREQUAL "RHEL")
                set(OPERATING_SYSTEM 0 CACHE STRING "${OPERATING_SYSTEM_STRING}")
            elseif(OS_ID STREQUAL "CENTOS")
                set(OPERATING_SYSTEM 0.1 CACHE STRING "${OPERATING_SYSTEM_STRING}")
            endif()
        else() # When there is no /etc/os-release file in Linux platforms
            message(FATAL_ERROR "/etc/os-release file does not exist, cannot determine the Linux distribution!")
        endif()

    elseif(UPPER_CMAKE_SYSTEM_NAME STREQUAL "MSYS") # MSYS2 is a POSIX-compatible environment, but users shall treat it as Windows for the purposes
        set(OPERATING_SYSTEM 1 CACHE STRING "${OPERATING_SYSTEM_STRING}")
    else()
        message(STATUS "Hello from subpage.cmake33 = ${CMAKE_SYSTEM_NAME}")
        set(OPERATING_SYSTEM 0 CACHE STRING "${OPERATING_SYSTEM_STRING}")
    endif()
    ###########################################################################################################
elseif(WIN32)
    set(OPERATING_SYSTEM 1 CACHE STRING "${OPERATING_SYSTEM_STRING}")
else()
    message(FATAL_ERROR "Unsupported platform! Only Linux and Windows are supported.")
endif()